#ifndef KAKOUNESESSION_HPP_INCLUDED
#define KAKOUNESESSION_HPP_INCLUDED

#include <string>

namespace domain
{

class KakouneSession
{
  public:
    virtual ~KakouneSession() = default;

    virtual std::string getSessionId() const = 0;
    virtual void setSessionId(const std::string &session_id) = 0;
};

} // namespace domain

#endif
