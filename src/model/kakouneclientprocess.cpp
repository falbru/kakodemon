#include "kakouneclientprocess.hpp"

#include <iostream>
#include <sys/poll.h>
#include <sys/wait.h>
#include <unistd.h>

#include <nlohmann/json.hpp>

#include "line.hpp"

KakouneClientProcess::KakouneClientProcess(const std::string &session_name) : m_session_name(session_name)
{
}

KakouneClientProcess::~KakouneClientProcess()
{
}

void KakouneClientProcess::start()
{
    if (pipe(m_pipefd) == 0)
    {
        // something worng
    }

    pid_t pid = fork();
    if (pid == -1)
    {
        // something wrong happened
    }

    if (pid == 0)
    {
        close(m_pipefd[0]);
        dup2(m_pipefd[1], STDOUT_FILENO);
        close(m_pipefd[1]);

        execlp("kak", "kak", "-ui", "json", "-c", m_session_name.c_str(), nullptr);
        perror("execlp");
        _exit(1);
    }
    else
    {
        close(m_pipefd[1]);
        m_pollfd.fd = m_pipefd[0];
        m_pollfd.events = POLLIN;
    }
}

void KakouneClientProcess::pollForRequests()
{
    int poll_result = poll(&m_pollfd, 1, 10);
    if (poll_result <= 0)
    {
        return;
    }

    std::string request_stream = m_request_remainder;
    ssize_t bytes_read = read(m_pipefd[0], m_buffer, sizeof(m_buffer) - 1);
    if (bytes_read > 0)
    {
        m_buffer[bytes_read] = '\0';
        request_stream += m_buffer;

        size_t newline_pos;
        while ((newline_pos = request_stream.find('\n')) != std::string::npos)
        {
            std::string json_request = request_stream.substr(0, newline_pos);

            if (!json_request.empty() && nlohmann::json::accept(json_request))
            {
                m_request_queue.push(json_request);
            }

            request_stream = request_stream.substr(newline_pos + 1);
        }
    }

    m_request_remainder = request_stream;
}

std::optional<KakouneClientRequest> KakouneClientProcess::getNextRequest()
{
    if (m_request_queue.empty())
    {
        return std::nullopt;
    }

    nlohmann::json data = nlohmann::json::parse(m_request_queue.front());
    m_request_queue.pop();

    auto request = KakouneClientRequest();
    if (data["method"] == "draw")
    {
        request.type = KakouneRequestType::DRAW;
        request.data = KakouneDrawRequestData{data["params"][0].get<std::vector<Line>>()};
        return request;
    }

    return std::nullopt;
}
