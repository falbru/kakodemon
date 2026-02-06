#include "commandcontroller.hpp"
#include "application/model/clientmanager.hpp"
#include <optional>

CommandController::CommandController()
    : m_command_interface(nullptr)
    , m_kakoune_session(nullptr)
{
}

void CommandController::init(CommandInterface *command_interface, ClientManager* client_manager, domain::KakouneSession *kakoune_session, Window *window)
{
    m_command_interface = command_interface;
    m_client_manager = client_manager;
    m_kakoune_session = kakoune_session;
    m_window = window;
}

void CommandController::update()
{
    auto commands = m_command_interface->getPendingCommands();
    for (const auto &command : commands)
    {
        if (command.name == "rename-session" && !command.args.empty())
        {
            m_window->setTitle(command.args[0]);
            m_kakoune_session->setSessionId(command.args[0]);
        }else if (command.name == "new-client") {
            if (command.args.empty()) {
                m_client_manager->createClient(std::nullopt, {});
            }else {
                std::string startup_command;
                for (const auto& str : command.args) {
                    startup_command += str;
                }

                m_client_manager->createClient(startup_command, {});
            }
        }
    }
}
