#include "focuscontroller.hpp"

void FocusController::init(KakouneClient** focused_client, LayoutController* layout_controller) {
    m_focused_client = focused_client;
    m_layout_controller = layout_controller;
}

void FocusController::onMouseMove(float x, float y) {
    if (m_is_mouse_pressed) {
        return;
    }

    ClientLayout* layout = m_layout_controller->findLayoutAt(x, y);
    if (layout && layout->client != *m_focused_client) {
        *m_focused_client = layout->client;
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
