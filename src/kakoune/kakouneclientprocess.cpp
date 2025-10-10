#include "kakouneclientprocess.hpp"

#include <sys/poll.h>
#include <sys/wait.h>
#include <unistd.h>

#include <nlohmann/json.hpp>

#include "kakoune/coord.hpp"
#include "kakoune/face.hpp"
#include "kakoune/menustyle.hpp"
#include "line.hpp"
#include "spdlog/spdlog.h"

KakouneClientProcess::KakouneClientProcess(const std::string &session_name) : m_session_name(session_name)
{
}

KakouneClientProcess::~KakouneClientProcess()
{
    if (m_stdin_pipefd[0] != -1)
    {
        close(m_stdin_pipefd[0]);
    }
    if (m_stdout_pipefd[1] != -1)
    {
        close(m_stdout_pipefd[1]);
    }
}

void KakouneClientProcess::start()
{
    if (pipe(m_stdout_pipefd) == 0)
    {
        // something worng
    }

    if (pipe(m_stdin_pipefd) != 0)
    {
        // something wrong
        return;
    }

    pid_t pid = fork();
    if (pid == -1)
    {
        // something wrong happened
    }

    if (pid == 0)
    {
        close(m_stdin_pipefd[1]);
        dup2(m_stdin_pipefd[0], STDIN_FILENO);
        close(m_stdin_pipefd[0]);

        close(m_stdout_pipefd[0]);
        dup2(m_stdout_pipefd[1], STDOUT_FILENO);
        close(m_stdout_pipefd[1]);

        execlp("kak", "kak", "-ui", "json", "-c", m_session_name.c_str(), nullptr);
        perror("execlp");
        _exit(1);
    }
    else
    {
        close(m_stdout_pipefd[1]);
        close(m_stdin_pipefd[0]);
        m_pollfd.fd = m_stdout_pipefd[0];
        m_pollfd.events = POLLIN;
    }
}

void KakouneClientProcess::setRequestCallback(std::function<void(const IncomingRequest&)> callback)
{
    m_request_callback = callback;
}

void KakouneClientProcess::pollForRequests()
{
    int poll_result = poll(&m_pollfd, 1, 10);
    if (poll_result <= 0)
    {
        return;
    }

    std::string request_stream = m_request_remainder;
    ssize_t bytes_read = read(m_stdout_pipefd[0], m_buffer, sizeof(m_buffer) - 1);
    if (bytes_read > 0)
    {
        m_buffer[bytes_read] = '\0';
        request_stream += m_buffer;

        size_t newline_pos;
        while ((newline_pos = request_stream.find('\n')) != std::string::npos)
        {
            std::string json_request = request_stream.substr(0, newline_pos);

            if (!json_request.empty())
            {
                std::optional<IncomingRequest> request = parseRequest(json_request);
                if (request.has_value() && m_request_callback)
                {
                    m_request_callback(request.value());
                }
            }

            request_stream = request_stream.substr(newline_pos + 1);
        }
    }

    m_request_remainder = request_stream;
}

void KakouneClientProcess::sendRequest(const OutgoingRequest& request)
{
    nlohmann::json data;
    data["jsonrpc"] = "2.0";

    if (request.type == OutgoingRequestType::KEYS) {
        data["method"] = "keys";
        data["params"] = std::get<KeysRequestData>(request.data).keys;
    }else if (request.type == OutgoingRequestType::RESIZE) {
        auto resize_data = std::get<ResizeRequestData>(request.data);

        data["method"] = "resize";
        data["params"] = {resize_data.rows, resize_data.columns};
    }

    std::string json_str = data.dump();

    ssize_t bytes_written = write(m_stdin_pipefd[1], json_str.c_str(), json_str.size());
    if (bytes_written == -1)
    {
        spdlog::error("Failed to write to Kakoune client");
    }
}

std::optional<IncomingRequest> KakouneClientProcess::parseRequest(std::string request) {
    nlohmann::json data = nlohmann::json::parse(request);

    std::string method = data["method"];
    nlohmann::json params = data["params"];

    auto parsed_request = IncomingRequest(); // TODO just use json deserialization
    if (method == "draw")
    {
        parsed_request.type = IncomingRequestType::DRAW;
        parsed_request.data = DrawRequestData{params[0].get<std::vector<kakoune::Line>>(), params[1].get<kakoune::Face>()};
        return parsed_request;
    }
    if (method == "draw_status")
    {
        parsed_request.type = IncomingRequestType::DRAW_STATUS;
        parsed_request.data = DrawStatusRequestData{params[0].get<kakoune::Line>(), params[1].get<kakoune::Line>(), params[2].get<kakoune::Face>()};
        return parsed_request;
    }
    if (method == "refresh")
    {
        parsed_request.type = IncomingRequestType::REFRESH;
        parsed_request.data = RefreshRequestData{params[0].get<bool>()};
        return parsed_request;
    }
    if (method == "menu_show")
    {
        parsed_request.type = IncomingRequestType::MENU_SHOW;

        parsed_request.data = MenuShowData{params[0].get<std::vector<kakoune::Line>>(), params[1].get<kakoune::Coord>(), params[2].get<kakoune::Face>(), params[3].get<kakoune::Face>(), params[4].get<kakoune::MenuStyle>()};
        return parsed_request;
    }
    if (method == "menu_hide")
    {
        parsed_request.type = IncomingRequestType::MENU_HIDE;
        parsed_request.data = {};
        return parsed_request;
    }
    if (method == "menu_select")
    {
        parsed_request.type = IncomingRequestType::MENU_SELECT;
        parsed_request.data = MenuSelectData{params[0].get<int>()};
        return parsed_request;
    }

    return std::nullopt;
}
