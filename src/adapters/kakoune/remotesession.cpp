#include "remotesession.hpp"

RemoteSession::RemoteSession(const std::string &session_id) : m_session_id(session_id)
{
}

std::string RemoteSession::getSessionId() const
{
    return m_session_id;
}

void RemoteSession::setSessionId(const std::string& session_id)
{
    m_session_id = session_id;
}

void RemoteSession::sendCommand(const std::string& command)
{
    std::string full_command = "kak -p " + m_session_id;
    FILE* pipe = popen(full_command.c_str(), "w");
    if (!pipe) {
        return;
    }
    fwrite(command.c_str(), 1, command.size(), pipe);
    pclose(pipe);
}
