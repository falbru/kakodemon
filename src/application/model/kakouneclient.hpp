#ifndef KAKOUNECLIENT_HPP_INCLUDED
#define KAKOUNECLIENT_HPP_INCLUDED

#include "domain/ports/kakouneinterface.hpp"
#include "domain/ports/kakounesession.hpp"
#include <memory>

class KakouneClient
{
  public:
    KakouneClient(domain::KakouneSession *session, std::unique_ptr<domain::KakouneInterface> interface);

    domain::KakouneState state;
    domain::KakouneSession *session;
    std::unique_ptr<domain::KakouneInterface> interface;
};

#endif
