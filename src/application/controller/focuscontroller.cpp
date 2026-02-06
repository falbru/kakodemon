#include "focuscontroller.hpp"
#include "application/model/clientmanager.hpp"
#include "application/model/kakouneclient.hpp"
#include "application/window.hpp"

void FocusController::init(KakouneClient** focused_client, ClientManager* client_manager, PaneLayout* layout_controller,
                           Window* window) {
    m_focused_client = focused_client;
    m_pane_layout = layout_controller;
    m_client_manager = client_manager;

    m_client_manager->onClientAdded([=](KakouneClient* client) {
        *m_focused_client = client;
    });

    m_client_manager->onClientRemoved([=](KakouneClient* client) {
        if (*m_focused_client == client) {
            auto& clients = m_client_manager->clients();
            *m_focused_client = clients.empty() ? nullptr : clients.front().get();
        }
    });

    window->onMouseMove([this](float x, float y) {
        onMouseMove(x, y);
    });

    window->onMouseButton([this](domain::MouseButtonEvent event) {
        onMouseButton(event);
    });
}

void FocusController::onMouseMove(float x, float y) {
    if (m_is_mouse_pressed) {
        return;
    }

    Pane* pane = m_pane_layout->findPaneAt(x, y);
    if (pane && pane->client != *m_focused_client) {
        *m_focused_client = pane->client;
    }
}

void FocusController::onMouseButton(domain::MouseButtonEvent event) {
    if (event.action == domain::MouseButtonAction::PRESS) {
        m_is_mouse_pressed = true;
    } else if (event.action == domain::MouseButtonAction::RELEASE) {
        m_is_mouse_pressed = false;
        onMouseMove(event.x, event.y);
    }
}
