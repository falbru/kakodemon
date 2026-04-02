#include "panelayout.hpp"
#include "application/model/clientmanager.hpp"
#include "application/model/kakouneclient.hpp"
#include <algorithm>

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

    if (m_layout_type == LayoutType::WIDE) {
        arrangeWide();
    } else {
        arrangeTall();
    }

    m_arrange_observers.notify(m_panes);
}

void PaneLayout::arrangeTall() {
    size_t master_count = std::min(static_cast<size_t>(m_num_masters), m_panes.size());
    size_t stack_count = m_panes.size() - master_count;

    if (stack_count == 0) {
        int total_height = m_bounds.height - static_cast<int>(BORDER_WIDTH) * (master_count - 1);
        int base_pane_height = total_height / master_count;
        int remainder = total_height % master_count;
        
        int y_pos = m_bounds.y;
        for (size_t i = 0; i < master_count; ++i) {
            int current_height = base_pane_height + (i < remainder ? 1 : 0);
            m_panes[i].bounds = {
                m_bounds.x,
                y_pos,
                m_bounds.width,
                current_height
            };
            y_pos += current_height + static_cast<int>(BORDER_WIDTH);
        }
        return;
    }

    float master_width = m_bounds.width * m_master_ratio;
    float stack_width = m_bounds.width - master_width - BORDER_WIDTH;
    float stack_x = m_bounds.x + master_width + BORDER_WIDTH;

    {
        int total_height = m_bounds.height - static_cast<int>(BORDER_WIDTH) * (master_count - 1);
        int base_pane_height = total_height / master_count;
        int remainder = total_height % master_count;
        
        int y_pos = m_bounds.y;
        for (size_t i = 0; i < master_count; ++i) {
            int current_height = base_pane_height + (i < remainder ? 1 : 0);
            m_panes[i].bounds = {
                m_bounds.x,
                y_pos,
                static_cast<int>(master_width),
                current_height
            };
            y_pos += current_height + static_cast<int>(BORDER_WIDTH);
        }
    }

    {
        int total_height = m_bounds.height - static_cast<int>(BORDER_WIDTH) * (stack_count - 1);
        int base_pane_height = total_height / stack_count;
        int remainder = total_height % stack_count;
        
        int y_pos = m_bounds.y;
        for (size_t i = 0; i < stack_count; ++i) {
            int current_height = base_pane_height + (i < remainder ? 1 : 0);
            m_panes[master_count + i].bounds = {
                static_cast<int>(stack_x),
                y_pos,
                static_cast<int>(stack_width),
                current_height
            };
            y_pos += current_height + static_cast<int>(BORDER_WIDTH);
        }
    }
}

void PaneLayout::arrangeWide() {
    size_t master_count = std::min(static_cast<size_t>(m_num_masters), m_panes.size());
    size_t stack_count = m_panes.size() - master_count;

    if (stack_count == 0) {
        int total_width = m_bounds.width - static_cast<int>(BORDER_WIDTH) * (master_count - 1);
        int base_pane_width = total_width / master_count;
        int remainder = total_width % master_count;
        
        int x_pos = m_bounds.x;
        for (size_t i = 0; i < master_count; ++i) {
            int current_width = base_pane_width + (i < remainder ? 1 : 0);
            m_panes[i].bounds = {
                x_pos,
                m_bounds.y,
                current_width,
                m_bounds.height
            };
            x_pos += current_width + static_cast<int>(BORDER_WIDTH);
        }
        return;
    }

    float master_height = m_bounds.height * m_master_ratio;
    float stack_height = m_bounds.height - master_height - BORDER_WIDTH;
    float stack_y = m_bounds.y + master_height + BORDER_WIDTH;

    {
        int total_width = m_bounds.width - static_cast<int>(BORDER_WIDTH) * (master_count - 1);
        int base_pane_width = total_width / master_count;
        int remainder = total_width % master_count;
        
        int x_pos = m_bounds.x;
        for (size_t i = 0; i < master_count; ++i) {
            int current_width = base_pane_width + (i < remainder ? 1 : 0);
            m_panes[i].bounds = {
                x_pos,
                m_bounds.y,
                current_width,
                static_cast<int>(master_height)
            };
            x_pos += current_width + static_cast<int>(BORDER_WIDTH);
        }
    }

    {
        int total_width = m_bounds.width - static_cast<int>(BORDER_WIDTH) * (stack_count - 1);
        int base_pane_width = total_width / stack_count;
        int remainder = total_width % stack_count;
        
        int x_pos = m_bounds.x;
        for (size_t i = 0; i < stack_count; ++i) {
            int current_width = base_pane_width + (i < remainder ? 1 : 0);
            m_panes[master_count + i].bounds = {
                x_pos,
                static_cast<int>(stack_y),
                current_width,
                static_cast<int>(stack_height)
            };
            x_pos += current_width + static_cast<int>(BORDER_WIDTH);
        }
    }
}

void PaneLayout::setLayoutType(LayoutType layout_type) {
    m_layout_type = layout_type;
}

LayoutType PaneLayout::getLayoutType() const {
    return m_layout_type;
}

void PaneLayout::setNumMasters(int num_masters) {
    m_num_masters = std::max(1, num_masters);
    m_num_masters_changed_observers.notify(m_num_masters);
}

int PaneLayout::getNumMasters() const {
    return m_num_masters;
}

void PaneLayout::setMasterRatio(float master_ratio) {
    m_master_ratio = std::max(0.1f, std::min(0.9f, master_ratio));
}

float PaneLayout::getMasterRatio() const {
    return m_master_ratio;
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

domain::ObserverId PaneLayout::onArrange(std::function<void(const std::vector<Pane>&)> callback) {
    return m_arrange_observers.addObserver(std::move(callback));
}

domain::ObserverId PaneLayout::onNumMastersChanged(std::function<void(int)> callback) {
    return m_num_masters_changed_observers.addObserver(std::move(callback));
}

void PaneLayout::removeObserver(domain::ObserverId id) {
    m_arrange_observers.removeObserver(id);
    m_num_masters_changed_observers.removeObserver(id);
}

void PaneLayout::setBounds(const domain::Rectangle& bounds) {
    m_bounds = bounds;
}

