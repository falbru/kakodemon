#include "localsession.hpp"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include "spdlog/spdlog.h"

LocalSession::LocalSession(const std::string &session_id) : m_session_id(session_id), m_daemon_pid(-1), m_session_started(false)
{
}

LocalSession::~LocalSession()
{
    if (!m_fifo_path.empty())
    {
        unlink(m_fifo_path.c_str());
    }

    if (m_session_started)
    {
        int pipefd[2];
        if (pipe(pipefd) == -1)
        {
            spdlog::error("Failed to create pipe for killing Kakoune session");
            return;
        }

        pid_t pid = fork();
        if (pid == -1)
        {
            spdlog::error("Failed to fork for killing Kakoune session");
            close(pipefd[0]);
            close(pipefd[1]);
            return;
        }

        if (pid == 0)
        {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);

            execlp("/usr/local/bin/kak", "kak", "-p", m_session_id.c_str(), nullptr);
            perror("execlp");
            _exit(1);
        }
        else
        {
            close(pipefd[0]);
            const char* kill_cmd = "kill!\n";
            write(pipefd[1], kill_cmd, 6);
            close(pipefd[1]);

            waitpid(pid, nullptr, 0);
            spdlog::info("Killed Kakoune session: {}", m_session_id);
        }
    }
}

void LocalSession::start()
{
    m_fifo_path = "/tmp/kakodemon_" + m_session_id + "_" + std::to_string(getpid()) + ".fifo";

    if (mkfifo(m_fifo_path.c_str(), 0600) == -1)
    {
        spdlog::error("Failed to create FIFO at {}", m_fifo_path);
        return;
    }

    pid_t pid = fork();
    if (pid == -1)
    {
        spdlog::error("Failed to fork for local Kakoune session");
        unlink(m_fifo_path.c_str());
        return;
    }

    if (pid == 0)
    {
        std::string init_command = "nop %sh{ echo > '" + m_fifo_path + "'}";
        execlp("/usr/local/bin/kak", "kak", "-d", "-s", m_session_id.c_str(), "-E", init_command.c_str(), nullptr);
        perror("execlp");
        _exit(1);
    }
    else
    {
        m_daemon_pid = pid;

        int fd = open(m_fifo_path.c_str(), O_RDONLY);
        if (fd == -1)
        {
            spdlog::error("Failed to open FIFO for reading");
            unlink(m_fifo_path.c_str());
            return;
        }

        char buffer[16];
        ssize_t bytes_read = read(fd, buffer, sizeof(buffer));
        close(fd);

        unlink(m_fifo_path.c_str());
        m_fifo_path.clear();

        if (bytes_read <= 0)
        {
            spdlog::error("Failed to read from FIFO or Kakoune didn't signal readiness");
        }
        else
        {
            m_session_started = true;
            spdlog::info("Kakoune daemon started successfully for session: {}", m_session_id);
        }
    }
}

std::string LocalSession::getSessionId() const
{
    return m_session_id;
}
