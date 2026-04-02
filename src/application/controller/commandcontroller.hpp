#ifndef COMMANDCONTROLLER_HPP_INCLUDED
#define COMMANDCONTROLLER_HPP_INCLUDED

#include "application/model/clientmanager.hpp"
#include "application/model/focusedclientstack.hpp"
#include "application/model/panelayout.hpp"
#include "domain/ports/commandinterface.hpp"
#include "domain/ports/kakounesession.hpp"
#include "domain/ports/window.hpp"

class CommandController
{
  public:
    CommandController();

    void init(domain::CommandInterface *command_interface, ClientManager *client_manager,
              domain::KakouneSession *kakoune_session, domain::Window *window, PaneLayout *pane_layout,
              FocusedClientStack *focused_client_stack);
    void update();

  private:
    domain::CommandInterface *m_command_interface;
    domain::KakouneSession *m_kakoune_session;
    ClientManager *m_client_manager;
    domain::Window *m_window;
    PaneLayout *m_pane_layout;
    FocusedClientStack *m_focused_client_stack;
};

#endif
