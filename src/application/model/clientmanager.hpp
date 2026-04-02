#ifndef MODEL_CLIENTMANAGER_HPP_INCLUDED
#define MODEL_CLIENTMANAGER_HPP_INCLUDED

#include "application/model/kakouneclient.hpp"
#include "domain/observerlist.hpp"
#include "domain/ports/kakounesession.hpp"

class ClientManager
{
  public:
    ClientManager(domain::KakouneSession *session);

    KakouneClient *createClient(std::optional<std::string> startup_command, std::vector<std::string> file_arguments);

    void setDefaultUIOptions(domain::UIOptions ui_options);
    void removeClient(KakouneClient *client);

    domain::ObserverId onClientAdded(std::function<void(KakouneClient *)> callback);
    domain::ObserverId onClientRemoved(std::function<void(KakouneClient *)> callback);
    domain::ObserverId onClientRenamed(std::function<void(KakouneClient *)> callback);
    void removeObserver(domain::ObserverId id);

    const std::vector<std::unique_ptr<KakouneClient>> &clients() const;
    KakouneClient *findClientById(int client_id);
    KakouneClient *findClientByName(const std::string &client_name);
    void renameClient(int client_id, const std::string &new_name);

  private:
    domain::KakouneSession *m_session;
    domain::UIOptions m_default_ui_options;
    std::vector<std::unique_ptr<KakouneClient>> m_clients;
    int m_next_client_id = 0;

    domain::ObserverList<KakouneClient *> m_client_added_observers;
    domain::ObserverList<KakouneClient *> m_client_removed_observers;
    domain::ObserverList<KakouneClient *> m_client_renamed_observers;
    int generateUniqueClientId();
};

#endif
