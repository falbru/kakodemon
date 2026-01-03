#include "kakouneclientprocess.hpp"

#include <exception>
#include <optional>
#include <sys/poll.h>
#include <sys/wait.h>
#include <unistd.h>

#include <nlohmann/json.hpp>

#include "adapters/kakoune/uioptions.hpp"
#include "coord.hpp"
#include "face.hpp"
#include "infostyle.hpp"
#include "menustyle.hpp"
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
    start(std::nullopt);
}

void KakouneClientProcess::start(std::optional<std::string> startup_command)
{
    start(startup_command, {});
}

void KakouneClientProcess::start(std::optional<std::string> startup_command, const std::vector<std::string>& file_arguments)
{
    if (pipe(m_stdout_pipefd) != 0)
    {
        throw std::runtime_error("stdout pipe creation failed");
    }

    if (pipe(m_stdin_pipefd) != 0)
    {
        throw std::runtime_error("stdin pipe creation failed");
    }

    pid_t pid = fork();
    if (pid == -1)
    {
        throw std::runtime_error("fork failed");
    }

    if (pid == 0)
    {
        close(m_stdin_pipefd[1]);
        dup2(m_stdin_pipefd[0], STDIN_FILENO);
        close(m_stdin_pipefd[0]);

        close(m_stdout_pipefd[0]);
        dup2(m_stdout_pipefd[1], STDOUT_FILENO);
        close(m_stdout_pipefd[1]);

        std::vector<const char*> args;
        args.push_back("kak");
        args.push_back("-ui");
        args.push_back("json");
        args.push_back("-c");
        args.push_back(m_session_name.c_str());

        if (startup_command.has_value()) {
            args.push_back("-e");
            args.push_back(startup_command->c_str());
        }

        for (const auto& file_arg : file_arguments) {
            args.push_back(file_arg.c_str());
        }

        args.push_back(nullptr);

        execv("/usr/local/bin/kak", const_cast<char* const*>(args.data()));

        perror("execv");
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

void KakouneClientProcess::setRequestCallback(std::function<void(const IncomingRequest &)> callback)
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

void KakouneClientProcess::sendRequest(const OutgoingRequest &request)
{
    nlohmann::json data;
    data["jsonrpc"] = "2.0";

    if (request.type == OutgoingRequestType::KEYS)
    {
        data["method"] = "keys";
        data["params"] = std::get<KeysRequestData>(request.data).keys;
    }
    else if (request.type == OutgoingRequestType::RESIZE)
    {
        auto resize_data = std::get<ResizeRequestData>(request.data);

        data["method"] = "resize";
        data["params"] = {resize_data.rows, resize_data.columns};
    }
    else if (request.type == OutgoingRequestType::MOUSE_MOVE)
    {
        auto mouse_move_data = std::get<MouseMoveRequestData>(request.data);

        data["method"] = "mouse_move";
        data["params"] = {mouse_move_data.line, mouse_move_data.column};
    }
    else if (request.type == OutgoingRequestType::MOUSE_PRESS)
    {
        auto mouse_press_data = std::get<MousePressRequestData>(request.data);

        data["method"] = "mouse_press";
        data["params"] = {mouse_press_data.button, mouse_press_data.line, mouse_press_data.column};
    }
    else if (request.type == OutgoingRequestType::MOUSE_RELEASE)
    {
        auto mouse_release_data = std::get<MouseReleaseRequestData>(request.data);

        data["method"] = "mouse_release";
        data["params"] = {mouse_release_data.button, mouse_release_data.line, mouse_release_data.column};
    }
    else if (request.type == OutgoingRequestType::MENU_SELECT)
    {
        auto menu_select_data = std::get<MenuSelectRequestData>(request.data);

        data["method"] = "menu_select";
        data["params"] = {menu_select_data.selected_index};
    }
    else if (request.type == OutgoingRequestType::SCROLL)
    {
        auto scroll_data = std::get<ScrollRequestData>(request.data);

        data["method"] = "scroll";
        data["params"] = {scroll_data.amount, scroll_data.line, scroll_data.line};
    }

    std::string json_str = data.dump() + "\n";

    ssize_t bytes_written = write(m_stdin_pipefd[1], json_str.c_str(), json_str.size());
    if (bytes_written == -1)
    {
        spdlog::error("Failed to write to Kakoune client");
    }
}

std::optional<IncomingRequest> KakouneClientProcess::parseRequest(std::string request)
{
    nlohmann::json data = nlohmann::json::parse(request);

    std::string method = data["method"];
    nlohmann::json params = data["params"];

    auto parsed_request = IncomingRequest(); // TODO just use json deserialization
    if (method == "draw")
    {
        parsed_request.type = IncomingRequestType::DRAW;
        parsed_request.data = DrawRequestData{params[0].get<std::vector<kakoune::Line>>(), params[1].get<kakoune::Coord>(), params[2].get<kakoune::Face>()};
        return parsed_request;
    }
    if (method == "draw_status")
    {
        parsed_request.type = IncomingRequestType::DRAW_STATUS;
        parsed_request.data = DrawStatusRequestData{params[0].get<kakoune::Line>(), params[1].get<kakoune::Line>(), params[2].get<int>(), params[3].get<kakoune::Line>(), params[4].get<kakoune::Face>()};
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

        parsed_request.data = MenuShowData{params[0].get<std::vector<kakoune::Line>>(), params[1].get<kakoune::Coord>(),
                                           params[2].get<kakoune::Face>(), params[3].get<kakoune::Face>(),
                                           params[4].get<kakoune::MenuStyle>()};
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
    if (method == "info_show")
    {
        parsed_request.type = IncomingRequestType::INFO_SHOW;
        parsed_request.data = InfoShowData{params[0].get<kakoune::Line>(), params[1].get<std::vector<kakoune::Line>>(),
                                           params[2].get<kakoune::Coord>(), params[3].get<kakoune::Face>(),
                                           params[4].get<kakoune::InfoStyle>()};
        return parsed_request;
    }
    if (method == "info_hide")
    {
        parsed_request.type = IncomingRequestType::INFO_HIDE;
        parsed_request.data = {};
        return parsed_request;
    }
    if (method == "set_ui_options")
    {
        parsed_request.type = IncomingRequestType::SET_UI_OPTIONS;
        parsed_request.data = UIOptionsData{params[0].get<kakoune::UIOptions>()};

        return parsed_request;
    }

    return std::nullopt;
}
