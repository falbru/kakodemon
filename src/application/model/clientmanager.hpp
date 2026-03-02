#ifndef MODEL_CLIENTMANAGER_HPP_INCLUDED
#define MODEL_CLIENTMANAGER_HPP_INCLUDED

#include "application/model/kakouneclient.hpp"
#include "application/observerlist.hpp"
#include "domain/ports/kakounesession.hpp"

class ClientManager
{
  public:
    ClientManager(domain::KakouneSession *session);

    KakouneClient *createClient(std::optional<std::string> startup_command, std::vector<std::string> file_arguments);

    void setDefaultUIOptions(domain::UIOptions ui_options);
    void removeClient(KakouneClient *client);

    ObserverId onClientAdded(std::function<void(KakouneClient *)> callback);
    ObserverId onClientRemoved(std::function<void(KakouneClient *)> callback);
    void removeObserver(ObserverId id);

    const std::vector<std::unique_ptr<KakouneClient>> &clients() const;

  private:
    domain::KakouneSession *m_session;
    domain::UIOptions m_default_ui_options;
    std::vector<std::unique_ptr<KakouneClient>> m_clients;

    ObserverList<KakouneClient *> m_client_added_observers;
    ObserverList<KakouneClient *> m_client_removed_observers;
};

#endif
