#include "panelayout.hpp"
#include "application/model/clientmanager.hpp"
#include "application/model/kakouneclient.hpp"

void PaneLayout::init(ClientManager* client_manager) {
    m_panes.clear();

    m_client_manager = client_manager;
    m_client_manager->onClientAdded([=](KakouneClient*) {
        arrange();
    });
}

void PaneLayout::arrange() {
    if (m_client_manager->clients().empty()) {
        return;
    }

    m_panes.clear();
    m_panes.reserve(m_client_manager->clients().size());
    for (auto& client : m_client_manager->clients()) {
        m_panes.push_back({client.get(), {0, 0, 0, 0}});
    }

    float pane_width = m_bounds.width / static_cast<float>(m_panes.size());

    for (size_t i = 0; i < m_panes.size(); ++i) {
        m_panes[i].bounds = {
            m_bounds.x + pane_width * static_cast<float>(i),
            m_bounds.y,
            pane_width,
            m_bounds.height
        };
    }

    notifyArrangeObservers();
}

Pane* PaneLayout::findPaneAt(float x, float y) {
    for (auto& pane : m_panes) {
        if (x >= pane.bounds.x && x < pane.bounds.x + pane.bounds.width &&
            y >= pane.bounds.y && y < pane.bounds.y + pane.bounds.height) {
            return &pane;
        }
    }
    return nullptr;
}

Pane* PaneLayout::findPaneForClient(KakouneClient* client) {
    for (auto& pane : m_panes) {
        if (pane.client == client) {
            return &pane;
        }
    }
    return nullptr;
}

const std::vector<Pane>& PaneLayout::getPanes() const {
    return m_panes;
}

ObserverId PaneLayout::onArrange(std::function<void(const std::vector<Pane>&)> callback) {
    ObserverId id = m_next_observer_id++;
    m_arrange_observers[id] = callback;
    return id;
}

void PaneLayout::removeObserver(ObserverId id) {
    m_arrange_observers.erase(id);
}

void PaneLayout::setBounds(const domain::Rectangle& bounds) {
    m_bounds = bounds;
}

void PaneLayout::notifyArrangeObservers() const {
    for (const auto& [id, callback] : m_arrange_observers) {
        callback(m_panes);
    }
}
