#ifndef COMMANDCONTROLLER_HPP_INCLUDED
#define COMMANDCONTROLLER_HPP_INCLUDED

#include "application/model/clientmanager.hpp"
#include "domain/ports/commandinterface.hpp"
#include "domain/ports/kakounesession.hpp"
#include <functional>
#include <string>

class CommandController
{
  public:
    CommandController();

    void init(CommandInterface *command_interface, ClientManager *client_manager,
              domain::KakouneSession *kakoune_session, std::function<void(const std::string &)> set_window_title);
    void update();

  private:
    CommandInterface *m_command_interface;
    domain::KakouneSession *m_kakoune_session;
    ClientManager *m_client_manager;
    std::function<void(const std::string &)> m_set_window_title;
};

#endif
