#ifndef COMMANDCONTROLLER_HPP_INCLUDED
#define COMMANDCONTROLLER_HPP_INCLUDED

#include "application/model/clientmanager.hpp"
#include "domain/ports/commandinterface.hpp"
#include "domain/ports/kakounesession.hpp"
#include "domain/ports/window.hpp"

class CommandController
{
  public:
    CommandController();

    void init(CommandInterface *command_interface, ClientManager *client_manager,
              domain::KakouneSession *kakoune_session, domain::Window *window);
    void update();

  private:
    CommandInterface *m_command_interface;
    domain::KakouneSession *m_kakoune_session;
    ClientManager *m_client_manager;
    domain::Window *m_window;
};

#endif
