#include "clientmanager.hpp"
#include "adapters/kakoune/jsonrpckakouneinterface.hpp"
#include "application/model/kakouneclient.hpp"
#include "domain/ports/kakounesession.hpp"
#include <algorithm>
#include <memory>

ClientManager::ClientManager(domain::KakouneSession* session) : m_session(session) {
}

KakouneClient* ClientManager::createClient(std::optional<std::string> startup_command, std::vector<std::string> file_arguments) {
    auto interface = std::make_unique<kakoune::JsonRpcKakouneInterface>(*m_session, startup_command, file_arguments);
    std::unique_ptr<KakouneClient> new_client = std::make_unique<KakouneClient>(m_session, std::move(interface));
    new_client->setUIOptions(m_default_ui_options);
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

ObserverId ClientManager::onClientAdded(std::function<void(KakouneClient *)> callback) {
    return m_client_added_observers.addObserver(std::move(callback));
}

ObserverId ClientManager::onClientRemoved(std::function<void(KakouneClient *)> callback) {
    return m_client_removed_observers.addObserver(std::move(callback));
}

void ClientManager::removeObserver(ObserverId id) {
    m_client_added_observers.removeObserver(id);
    m_client_removed_observers.removeObserver(id);
}

const std::vector<std::unique_ptr<KakouneClient>>& ClientManager::clients() const {
    return m_clients;
}
