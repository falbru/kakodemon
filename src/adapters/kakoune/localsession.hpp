#ifndef LOCALSESSION_HPP_INCLUDED
#define LOCALSESSION_HPP_INCLUDED

#include "domain/ports/kakounesession.hpp"
#include <string>
#include <sys/types.h>

class LocalSession : public domain::KakouneSession
{
  public:
    explicit LocalSession(const std::string &session_id);
    ~LocalSession();

    void start();

    std::string getSessionId() const override;

  private:
    std::string m_session_id;
    pid_t m_daemon_pid;
    std::string m_fifo_path;
    bool m_session_started;
};

#endif
