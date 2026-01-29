#include "panelayout.hpp"

void PaneLayout::init(std::vector<std::unique_ptr<KakouneClient>>* clients) {
    m_clients = clients;
    m_panes.clear();
}

void PaneLayout::arrange(float width, float height) {
    if (m_clients->empty()) {
        return;
    }

    m_panes.clear();
    m_panes.reserve(m_clients->size());
    for (auto& client : *m_clients) {
        m_panes.push_back({client.get(), {0, 0, 0, 0}});
    }

    float pane_width = width / static_cast<float>(m_panes.size());

    for (size_t i = 0; i < m_panes.size(); ++i) {
        m_panes[i].bounds = {
            pane_width * static_cast<float>(i),
            0,
            pane_width,
            height
        };
    }

    notifyArrangeCallbacks();
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

void PaneLayout::addArrangeCallback(std::function<void(const std::vector<Pane>&)> callback) {
    m_callbacks.push_back(callback);
}

void PaneLayout::notifyArrangeCallbacks() const {
    for (const auto& callback : m_callbacks) {
        callback(m_panes);
    }
    
}
