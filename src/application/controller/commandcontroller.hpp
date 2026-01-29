#ifndef COMMANDCONTROLLER_HPP_INCLUDED
#define COMMANDCONTROLLER_HPP_INCLUDED

#include "domain/ports/commandinterface.hpp"
#include "domain/ports/kakounesession.hpp"
#include <functional>
#include <optional>
#include <string>
#include <vector>

class CommandController
{
  public:
    CommandController();

    void init(CommandInterface *command_interface,
              std::function<void(std::optional<std::string>, std::vector<std::string>)> create_kakoune_client,
              domain::KakouneSession *kakoune_session, std::function<void(const std::string &)> set_window_title);
    void update();

  private:
    CommandInterface *m_command_interface;
    domain::KakouneSession *m_kakoune_session;
    std::function<void(const std::string &)> m_set_window_title;
    std::function<void(std::optional<std::string>, std::vector<std::string>)> m_create_kakoune_client;
};

#endif
