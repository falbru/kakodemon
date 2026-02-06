#include "namedpipecommandinterface.hpp"

#include <csignal>
#include <fcntl.h>
#include <stdexcept>
#include <sys/poll.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sstream>

#include "nlohmann/json.hpp"
#include "spdlog/spdlog.h"

NamedPipeCommandInterface::NamedPipeCommandInterface(const std::string& pipe_id, PipeMode mode)
    : m_pipe_path("/tmp/kakod-" + pipe_id), m_mode(mode), m_running(false), m_ready(false) {}

NamedPipeCommandInterface::~NamedPipeCommandInterface() {
    if (m_mode == PipeMode::Receive || m_mode == PipeMode::Both) {
        m_running.store(false);

        int fd = open(m_pipe_path.c_str(), O_WRONLY | O_NONBLOCK);
        if (fd != -1) {
            write(fd, "\n", 1);
            close(fd);
        }

        if (m_read_thread.joinable()) {
            m_read_thread.join();
        }

        unlink(m_pipe_path.c_str());
    }
}

void NamedPipeCommandInterface::init() {
    if (m_mode == PipeMode::Send) {
        return;
    }

    unlink(m_pipe_path.c_str());

    if (mkfifo(m_pipe_path.c_str(), 0666) == -1) {
        throw std::runtime_error("Failed to create named pipe at " + m_pipe_path);
    }

    m_running.store(true);
    m_read_thread = std::thread(&NamedPipeCommandInterface::readLoop, this);

    while (!m_ready.load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

ObserverId NamedPipeCommandInterface::onCommandReceived(std::function<void(const Command &)> callback) {
    ObserverId id = m_next_observer_id++;
    m_command_observers[id] = callback;
    return id;
}

void NamedPipeCommandInterface::removeCommandObserver(ObserverId id) {
    m_command_observers.erase(id);
}

void NamedPipeCommandInterface::notifyCommandObservers(const Command &command) {
    auto observers_copy = m_command_observers;
    for (auto &[id, callback] : observers_copy) {
        callback(command);
    }
}

void NamedPipeCommandInterface::readLoop() {
    if (m_mode == PipeMode::Send) {
        return;
    }

    int fd = open(m_pipe_path.c_str(), O_RDWR | O_NONBLOCK);
    if (fd == -1) {
        spdlog::error("Failed to open named pipe for reading: {}", strerror(errno));
        return;
    }

    m_ready.store(true);

    pollfd fds;
    fds.fd = fd;
    fds.events = POLL_IN;

    while (m_running.load()) {
        int ret = poll(&fds, 1, 500);

        if (ret == -1) {
            throw std::runtime_error("poll() failed: " + std::string(strerror(errno)));
        }

        if (ret == 0) {
            continue;
        }

        if (fds.revents & (POLLERR | POLLNVAL)) {
            throw std::runtime_error("poll error on pipe");
            break;
        }

        if ((fds.revents & POLL_IN) == 0) {
            continue;

        }

        std::string buffer;
        char chunk[256];
        ssize_t bytes_read;
        while ((bytes_read = read(fd, chunk, sizeof(chunk) - 1)) > 0) {
            chunk[bytes_read] = '\0';
            buffer += chunk;

            if (strchr(chunk, '\n') != nullptr) {
                break;
            }
        }

        if (bytes_read == 0) {
            close(fd);
            fd = open(m_pipe_path.c_str(), O_RDONLY);
            if (fd == -1) {
                spdlog::error("Failed to open named pipe for reading: {}", strerror(errno));
                return;
            }
        }

        if (bytes_read == -1 && errno != EAGAIN && errno != EWOULDBLOCK) {
            spdlog::error("read() failed: {}", strerror(errno));
            break;
        }

        if (buffer.empty()) {
            continue;
        }

        std::istringstream stream(buffer);
        std::string line;
        while (std::getline(stream, line)) {
            if (line.empty()) {
                continue;
            }
            try {
                nlohmann::json json = nlohmann::json::parse(line);
                Command cmd;
                cmd.name = json.at("command").get<std::string>();
                if (json.contains("args")) {
                    cmd.args = json.at("args").get<std::vector<std::string>>();
                }
                {
                    std::lock_guard<std::mutex> lock(m_commands_mutex);
                    m_pending_commands.push_back(cmd);
                }
                notifyCommandObservers(cmd);
            } catch (const nlohmann::json::exception& e) {
                spdlog::warn("Failed to parse command JSON: {}", e.what());
            }
        }
    }

    close(fd);
}

std::vector<Command> NamedPipeCommandInterface::getPendingCommands() {
    if (m_mode == PipeMode::Send) {
        return {};
    }

    std::lock_guard<std::mutex> lock(m_commands_mutex);
    std::vector<Command> commands = std::move(m_pending_commands);
    m_pending_commands.clear();
    return commands;
}

bool NamedPipeCommandInterface::sendCommand(const Command& command) {
    if (m_mode == PipeMode::Receive) {
        return false;
    }

    int fd = open(m_pipe_path.c_str(), O_WRONLY | O_NONBLOCK);
    if (fd == -1) {
        return false;
    }

    nlohmann::json json;
    json["command"] = command.name;
    if (!command.args.empty()) {
        json["args"] = command.args;
    }

    std::string message = json.dump() + "\n";
    ssize_t written = write(fd, message.c_str(), message.size());
    close(fd);

    return written != -1;
}
