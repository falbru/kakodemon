#include "infoboxcontroller.hpp"
#include "application/model/panelayout.hpp"

InfoBoxController::InfoBoxController() {

}

void InfoBoxController::init(KakouneClient** focused_client, PaneLayout* layout_controller, Window* window, domain::FontManager* font_manager, InfoBoxView* info_box_view) {
    m_focused_client = focused_client;
    m_pane_layout = layout_controller;
    m_window = window;
    m_font_manager = font_manager;
    m_info_box_view = info_box_view;
}

void InfoBoxController::handleMouseScroll(int scroll_amount)
{
    if (!*m_focused_client || !(*m_focused_client)->state.info_box.has_value()) return;

    m_info_box_view->handleMouseScroll((*m_focused_client)->info_box_state, scroll_amount);

    m_window->setNeedsRerender();
}

float InfoBoxController::x() const {
    return m_info_box_view->x();
}

float InfoBoxController::y() const {
    return m_info_box_view->y();
}

float InfoBoxController::width() const {
    return m_info_box_view->width();
}

float InfoBoxController::height() const {
    return m_info_box_view->height();
}
