#ifndef REMOTESESSION_HPP_INCLUDED
#define REMOTESESSION_HPP_INCLUDED

#include "domain/ports/kakounesession.hpp"
#include <string>

class RemoteSession : public domain::KakouneSession
{
  public:
    explicit RemoteSession(const std::string &session_id);

    std::string getSessionId() const override;
    void setSessionId(const std::string &session_id) override;
    void sendCommand(const std::string &command) override;

  private:
    std::string m_session_id;
};

#endif
