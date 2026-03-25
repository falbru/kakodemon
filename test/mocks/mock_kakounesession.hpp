#ifndef MOCK_KAKOUNESESSION_HPP_INCLUDED
#define MOCK_KAKOUNESESSION_HPP_INCLUDED

#include "domain/ports/kakounesession.hpp"
#include <string>
#include <vector>

class MockKakouneSession : public domain::KakouneSession
{
  public:
    std::string getSessionId() const override
    {
        return "test_session";
    }
    void setSessionId(const std::string &session_id) override
    {
    }
    void sendCommand(const std::string &command) override
    {
        commands_sent.push_back(command);
    }

    std::vector<std::string> commands_sent;
};

#endif