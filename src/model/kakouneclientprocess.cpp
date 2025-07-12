#include "kakouneclientprocess.hpp"

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

bool KakouneClientProcess::pollForRequest()
{
    int poll_result = poll(&m_pollfd, 1, 10);
    if (poll_result == 0)
    {
        return false;
    }

    ssize_t bytes_read;
    std::string line = m_remainder;

    while ((bytes_read = read(m_pipefd[0], m_buffer, sizeof(m_buffer) - 1)) > 0)
    {
        m_buffer[bytes_read] = '\0';
        line += m_buffer;

        size_t newline_pos = line.find('\n');
        if (newline_pos != std::string::npos)
        {
            std::string json_line = line.substr(0, newline_pos);

            m_remainder = line.substr(newline_pos + 1);

            if (nlohmann::json::accept(json_line))
            {
                m_request = json_line;
                return true;
            }

            // error
            return false;
        }
    }

    return false;
}

KakouneClientRequest KakouneClientProcess::getRequest()
{
    nlohmann::json data = nlohmann::json::parse(m_request);

    auto request = KakouneClientRequest();
    if (data["method"] == "draw")
    {
        request.type = KakouneRequestType::DRAW;
        request.data = KakouneDrawRequestData{data["params"][0].get<std::vector<Line>>()};
    }

    return request;
}
