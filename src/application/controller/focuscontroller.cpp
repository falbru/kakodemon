#include "focuscontroller.hpp"
#include "application/model/clientmanager.hpp"
#include "application/model/focusedclientstack.hpp"
#include "application/model/kakouneclient.hpp"
#include "application/view/multistyledmenu.hpp"
#include "domain/mouse.hpp"

void FocusController::init(FocusedClientStack *focused_client_stack, ClientManager *client_manager,
                           PaneLayout *layout_controller, domain::Window *window, MultiStyledMenuView* menu_view) {
    m_focused_client_stack = focused_client_stack;
    m_pane_layout = layout_controller;
    m_client_manager = client_manager;
    m_menu_view = menu_view;

    m_client_manager->onClientAdded([this](KakouneClient *client) {
        m_focused_client_stack->focus(client);
    });

    m_client_manager->onClientRemoved([this](KakouneClient *client) {
        m_focused_client_stack->remove(client);
    });

    window->onMouseMove([this](const domain::MouseMoveEvent& event) {
        onMouseMove(event.x, event.y);
    });

    window->onMouseButton([this](const domain::MouseButtonEvent& event) {
        onMouseButton(event);
    });
}

void FocusController::onMouseMove(float x, float y) {
    if (m_is_mouse_pressed) {
        return;
    }

    if (m_menu_view->isVisible()) {
        return;
    }

    Pane *pane = m_pane_layout->findPaneAt(x, y);
    if (pane && pane->client != m_focused_client_stack->focused()) {
        m_focused_client_stack->focus(pane->client);
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
