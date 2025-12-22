#ifndef KAKOUNECLIENT_HPP_INCLUDED
#define KAKOUNECLIENT_HPP_INCLUDED

#include "domain/kakounesession.hpp"
#include "domain/ports/kakouneinterface.hpp"
#include <memory>

class KakouneClient
{
  public:
    KakouneClient(std::unique_ptr<domain::KakouneSession> session, std::unique_ptr<domain::KakouneInterface> interface);

    domain::KakouneState state;
    std::unique_ptr<domain::KakouneSession> session;
    std::unique_ptr<domain::KakouneInterface> interface;
};

#endif
