#include "domain/ports/kakouneinterface.hpp"

namespace domain {

KakouneInterface::KakouneInterface(const KakouneSession& session) : m_session_id(session.getSessionId()) {

}

}
