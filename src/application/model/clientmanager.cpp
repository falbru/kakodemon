#include "clientmanager.hpp"
#include "adapters/kakoune/jsonrpckakouneinterface.hpp"
#include "application/model/kakouneclient.hpp"
#include "domain/ports/kakounesession.hpp"
#include <algorithm>
#include <memory>

ClientManager::ClientManager(domain::KakouneSession* session) : m_session(session) {
}

int ClientManager::generateUniqueClientId() {
    return m_next_client_id++;
}

KakouneClient* ClientManager::createClient(std::optional<std::string> startup_command, std::vector<std::string> file_arguments) {
    int client_id = generateUniqueClientId();
    auto interface = std::make_unique<kakoune::JsonRpcKakouneInterface>(*m_session, client_id, startup_command, file_arguments);
    std::unique_ptr<KakouneClient> new_client = std::make_unique<KakouneClient>(m_session, std::move(interface));
    new_client->setUIOptions(m_default_ui_options);

    new_client->client_id = client_id;
    new_client->client_name = "";

    KakouneClient* new_client_ptr = new_client.get();

    m_clients.push_back(std::move(new_client));

    m_client_added_observers.notify(new_client_ptr);

    return new_client_ptr;
}

void ClientManager::setDefaultUIOptions(domain::UIOptions ui_options) {
    m_default_ui_options = std::move(ui_options);
}

void ClientManager::removeClient(KakouneClient *client) {
    auto it = std::find_if(m_clients.begin(), m_clients.end(), [client](const auto& c) { return c.get() == client; });

    if (it != m_clients.end()) {
        auto client_unique_ptr = std::move(*it);
        m_clients.erase(it);
        m_client_removed_observers.notify(client_unique_ptr.get());
    }
}

domain::ObserverId ClientManager::onClientAdded(std::function<void(KakouneClient *)> callback) {
    return m_client_added_observers.addObserver(std::move(callback));
}

domain::ObserverId ClientManager::onClientRemoved(std::function<void(KakouneClient *)> callback) {
    return m_client_removed_observers.addObserver(std::move(callback));
}

domain::ObserverId ClientManager::onClientRenamed(std::function<void(KakouneClient *)> callback) {
    return m_client_renamed_observers.addObserver(std::move(callback));
}

void ClientManager::removeObserver(domain::ObserverId id) {
    m_client_added_observers.removeObserver(id);
    m_client_removed_observers.removeObserver(id);
    m_client_renamed_observers.removeObserver(id);
}

const std::vector<std::unique_ptr<KakouneClient>>& ClientManager::clients() const {
    return m_clients;
}

KakouneClient* ClientManager::findClientById(int client_id) {
    for (auto& client : m_clients) {
        if (client->client_id == client_id) {
            return client.get();
        }
    }
    return nullptr;
}

KakouneClient* ClientManager::findClientByName(const std::string& client_name) {
    for (auto& client : m_clients) {
        if (client->client_name == client_name) {
            return client.get();
        }
    }
    return nullptr;
}

void ClientManager::renameClient(int client_id, const std::string& new_name) {
    KakouneClient* client = findClientById(client_id);
    if (client) {
        client->client_name = new_name;
        m_client_renamed_observers.notify(client);
    }
}
