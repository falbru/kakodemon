#ifndef COMMANDCONTROLLER_HPP_INCLUDED
#define COMMANDCONTROLLER_HPP_INCLUDED

#include "application/model/clientmanager.hpp"
#include "application/window.hpp"
#include "domain/ports/commandinterface.hpp"
#include "domain/ports/kakounesession.hpp"

class CommandController
{
  public:
    CommandController();

    void init(CommandInterface *command_interface, ClientManager *client_manager,
              domain::KakouneSession *kakoune_session, Window *window);
    void update();

  private:
    CommandInterface *m_command_interface;
    domain::KakouneSession *m_kakoune_session;
    ClientManager *m_client_manager;
    Window *m_window;
};

#endif
