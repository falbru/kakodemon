#include "layoutcontroller.hpp"

void LayoutController::init(std::vector<std::unique_ptr<KakouneClient>>* clients) {
    m_clients = clients;
    m_layouts.clear();

    for (auto& client : *clients) {
        m_layouts.push_back({client.get(), {0, 0, 0, 0}});
    }
}

void LayoutController::arrange(float width, float height) {
    m_layouts.clear();

    for (auto& client : *m_clients) {
        m_layouts.push_back({client.get(), {0, 0, 0, 0}});
    }

    if (m_layouts.empty()) {
        return;
    }

    float client_width = width / static_cast<float>(m_layouts.size());

    for (size_t i = 0; i < m_layouts.size(); ++i) {
        m_layouts[i].bounds = {
            client_width * static_cast<float>(i),
            0,
            client_width,
            height
        };
    }
}

ClientLayout* LayoutController::findLayoutAt(float x, float y) {
    for (auto& layout : m_layouts) {
        if (x >= layout.bounds.x && x < layout.bounds.x + layout.bounds.width &&
            y >= layout.bounds.y && y < layout.bounds.y + layout.bounds.height) {
            return &layout;
        }
    }
    return nullptr;
}

ClientLayout* LayoutController::findLayoutForClient(KakouneClient* client) {
    for (auto& layout : m_layouts) {
        if (layout.client == client) {
            return &layout;
        }
    }
    return nullptr;
}

const std::vector<ClientLayout>& LayoutController::getLayouts() const {
    return m_layouts;
}
