#ifndef KAKOUNECLIENT_HPP_INCLUDED
#define KAKOUNECLIENT_HPP_INCLUDED

#include "application/model/viewstate.hpp"
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

    MenuViewState prompt_menu_state;
    MenuViewState inline_menu_state;
    MenuViewState search_menu_state;
    InputViewState status_line_state;
    InfoBoxViewState info_box_state;
};

#endif
