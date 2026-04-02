#include "commandcontroller.hpp"
#include "application/model/clientmanager.hpp"
#include <optional>

CommandController::CommandController()
    : m_command_interface(nullptr)
    , m_kakoune_session(nullptr)
{
}

void CommandController::init(domain::CommandInterface *command_interface, ClientManager* client_manager, domain::KakouneSession *kakoune_session, domain::Window *window, PaneLayout *pane_layout, FocusedClientStack *focused_client_stack)
{
    m_command_interface = command_interface;
    m_client_manager = client_manager;
    m_kakoune_session = kakoune_session;
    m_window = window;
    m_pane_layout = pane_layout;
    m_focused_client_stack = focused_client_stack;
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
        }else if (command.name == "set-layout" && !command.args.empty()) {
            if (command.args[0] == "tall") {
                m_pane_layout->setLayoutType(LayoutType::TALL);
            } else if (command.args[0] == "wide") {
                m_pane_layout->setLayoutType(LayoutType::WIDE);
            } else if (command.args[0] == "full") {
                m_pane_layout->setLayoutType(LayoutType::FULL);
            }
            m_pane_layout->arrange();
            m_window->setNeedsRerender();
        }else if (command.name == "new-client") {
            if (command.args.empty()) {
                m_client_manager->createClient(std::nullopt, {});
            }else {
                std::string startup_command;
                for (size_t i = 0; i < command.args.size(); i++) {
                    if (i != 0) {
                        startup_command += " ";
                    }
                    startup_command += command.args[i];
                }

                m_client_manager->createClient(startup_command, {});
            }
        }else if (command.name == "rename-client" && !command.args.empty()) {
            if (command.args.size() >= 2) {
                try {
                    int client_id = std::stoi(command.args[0]);
                    std::string new_name = command.args[1];
                    m_client_manager->renameClient(client_id, new_name);
                } catch (const std::exception&) {
                }
            }
        }else if (command.name == "focus" && !command.args.empty()) {
            std::string client_name = command.args[0];
            KakouneClient* client = m_client_manager->findClientByName(client_name);
            if (client) {
                m_focused_client_stack->focus(client);
            }
        }
    }
}
