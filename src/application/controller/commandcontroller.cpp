#include "commandcontroller.hpp"
#include <optional>

CommandController::CommandController()
    : m_command_interface(nullptr)
    , m_kakoune_session(nullptr)
{
}

void CommandController::init(CommandInterface *command_interface, std::function<void(std::optional<std::string>, std::vector<std::string>)> create_kakoune_client, domain::KakouneSession *kakoune_session,
                             std::function<void(const std::string &)> set_window_title)
{
    m_command_interface = command_interface;
    m_create_kakoune_client = create_kakoune_client;
    m_kakoune_session = kakoune_session;
    m_set_window_title = set_window_title;
}

void CommandController::update()
{
    auto commands = m_command_interface->getPendingCommands();
    for (const auto &command : commands)
    {
        if (command.name == "rename-session" && !command.args.empty())
        {
            m_set_window_title(command.args[0]);
            m_kakoune_session->setSessionId(command.args[0]);
        }else if (command.name == "new-client") {
            if (command.args.empty()) {
                m_create_kakoune_client(std::nullopt, {});
            }else {
                std::string startup_command;
                for (const auto& str : command.args) {
                    startup_command += str;
                }

                m_create_kakoune_client(startup_command, {});
            }
        }
    }
}
