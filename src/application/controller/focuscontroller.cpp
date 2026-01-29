#include "focuscontroller.hpp"

void FocusController::init(KakouneClient** focused_client, PaneLayout* layout_controller) {
    m_focused_client = focused_client;
    m_pane_layout = layout_controller;
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
