#include "remotesession.hpp"

RemoteSession::RemoteSession(const std::string &session_id) : m_session_id(session_id)
{
}

std::string RemoteSession::getSessionId() const
{
    return m_session_id;
}
