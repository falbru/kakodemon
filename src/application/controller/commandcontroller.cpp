#include "commandcontroller.hpp"

CommandController::CommandController()
    : m_command_interface(nullptr)
    , m_kakoune_session(nullptr)
{
}

void CommandController::init(CommandInterface *command_interface, domain::KakouneSession *kakoune_session,
                             std::function<void(const std::string &)> set_window_title)
{
    m_command_interface = command_interface;
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
        }
    }
}
