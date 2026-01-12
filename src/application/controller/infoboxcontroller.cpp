#include "infoboxcontroller.hpp"
#include "application/model/uioptions.hpp"

InfoBoxController::InfoBoxController() {

}

void InfoBoxController::init(KakouneClient* kakoune_client, EditorController* editor_controller, domain::FontManager* font_manager, InfoBoxView* info_box_view, std::function<void()> mark_dirty) {
    m_kakoune_client = kakoune_client;
    m_editor_controller = editor_controller;
    m_font_manager = font_manager;
    m_info_box_view = info_box_view;
    m_mark_dirty = mark_dirty;
}

void InfoBoxController::update(const UIOptions& ui_options) {
}

void InfoBoxController::render(const UIOptions& ui_options) {
    if (!m_kakoune_client->state.info_box.has_value() || (m_kakoune_client->state.info_box->title.size() == 0 && m_kakoune_client->state.info_box->content.size() == 0)) return;

    m_info_box_view->render(m_kakoune_client, m_font_manager, ui_options, m_editor_controller->width(), m_editor_controller->height());
}

void InfoBoxController::onMouseScroll(int scroll_amount)
{
    if (!m_kakoune_client->state.info_box.has_value()) return;

    m_info_box_view->onMouseScroll(scroll_amount);

    if (m_mark_dirty) {
        m_mark_dirty();
    }
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
