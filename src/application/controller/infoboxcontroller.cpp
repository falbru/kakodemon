#include "infoboxcontroller.hpp"
#include "application/view/rendercontext.hpp"
#include "domain/uioptions.hpp"

InfoBoxController::InfoBoxController() {

}

void InfoBoxController::init(KakouneClient** focused_client, EditorController* editor_controller, domain::FontManager* font_manager, InfoBoxView* info_box_view, std::function<void()> mark_dirty) {
    m_focused_client = focused_client;
    m_editor_controller = editor_controller;
    m_font_manager = font_manager;
    m_info_box_view = info_box_view;
    m_mark_dirty = mark_dirty;
}

void InfoBoxController::update(const domain::UIOptions& ui_options) {
}

void InfoBoxController::render(const domain::UIOptions& ui_options) {
    if (!(*m_focused_client)->state.info_box.has_value() || ((*m_focused_client)->state.info_box->title.size() == 0 && (*m_focused_client)->state.info_box->content.size() == 0)) return;

    RenderContext render_context = {
        m_font_manager,
        (*m_focused_client)->state.default_face,
        ui_options,
        static_cast<float>(m_editor_controller->width()),
        static_cast<float>(m_editor_controller->height())
    };

    m_info_box_view->render(render_context, (*m_focused_client)->info_box_state, *(*m_focused_client)->state.info_box, (*m_focused_client)->state.cursor_position);
}

void InfoBoxController::onMouseScroll(int scroll_amount)
{
    if (!(*m_focused_client)->state.info_box.has_value()) return;

    m_info_box_view->onMouseScroll((*m_focused_client)->info_box_state, scroll_amount);

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
