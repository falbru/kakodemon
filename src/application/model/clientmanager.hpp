#ifndef MODEL_CLIENTMANAGER_HPP_INCLUDED
#define MODEL_CLIENTMANAGER_HPP_INCLUDED

#include "application/model/kakouneclient.hpp"
#include "domain/ports/kakounesession.hpp"
#include <map>

using ObserverId = int;

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
    void notifyClientAddedObservers(KakouneClient *client);
    void notifyClientRemovedObservers(KakouneClient *client);

    domain::KakouneSession *m_session;
    domain::UIOptions m_default_ui_options;
    std::vector<std::unique_ptr<KakouneClient>> m_clients;

    ObserverId m_next_observer_id = 0;
    std::map<ObserverId, std::function<void(KakouneClient *)>> m_client_added_observers;
    std::map<ObserverId, std::function<void(KakouneClient *)>> m_client_removed_observers;
};

#endif
