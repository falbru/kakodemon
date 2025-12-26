#include "infoboxcontroller.hpp"
#include "application/model/uioptions.hpp"

InfoBoxController::InfoBoxController() {

}

void InfoBoxController::init(KakouneClient* kakoune_client, EditorController* editor_controller, domain::FontManager* font_manager, InfoBoxView* info_box_view) {
    m_kakoune_client = kakoune_client;
    m_editor_controller = editor_controller;
    m_font_manager = font_manager;
    m_info_box_view = info_box_view;
}

void InfoBoxController::update(const UIOptions& ui_options) {
}

void InfoBoxController::render(const UIOptions& ui_options) {
    if (!m_kakoune_client->state.info_box.has_value()) return;

    m_info_box_view->render(m_kakoune_client, m_font_manager, ui_options, m_editor_controller->width(), m_editor_controller->height());
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
