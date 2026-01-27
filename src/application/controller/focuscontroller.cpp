#include "focuscontroller.hpp"

void FocusController::init(KakouneClient** focused_client, LayoutController* layout_controller) {
    m_focused_client = focused_client;
    m_layout_controller = layout_controller;
}

void FocusController::onMouseMove(float x, float y) {
    ClientLayout* layout = m_layout_controller->findLayoutAt(x, y);
    if (layout && layout->client != *m_focused_client) {
        *m_focused_client = layout->client;
    }
}
