#include "editorcontroller.hpp"
#include "application/model/uioptions.hpp"
#include "domain/color.hpp"
#include "domain/mouse.hpp"
#include "application/view/statusbar.hpp"
#include <memory>

EditorController::EditorController()
{
}

void EditorController::init(KakouneClient* kakoune_client,
                            KakouneContentView* kakoune_content_view,
                            StatusBarView* status_bar_view,
                            domain::FontManager* font_manager,
                            std::function<void(domain::RGBAColor)> set_clear_color)
{
    m_kakoune_client = kakoune_client;
    m_kakoune_content_view = kakoune_content_view;
    m_status_bar_view = status_bar_view;
    m_font_manager = font_manager;
    m_set_clear_color = set_clear_color;
}

void EditorController::update(UIOptions& ui_options)
{
    auto frame_state = m_kakoune_client->interface->getNextKakouneState();
    if (frame_state.has_value()) {
        m_kakoune_client->state = frame_state.value();
        setClearColor(m_kakoune_client->state.default_face.getBg());
    }

    auto font_option = m_kakoune_client->interface->getUIOptionsFont();
    if (font_option.has_value()) {
        domain::Font* prev_font = ui_options.font;
        ui_options.font = m_font_manager->getFontFromName(font_option.value());
        if (prev_font != ui_options.font) {
            loadBasicGlyphs(ui_options.font);
        }
    }
}

void EditorController::render(const UIOptions& ui_options)
{
    m_kakoune_content_view->render(ui_options.font, m_font_manager, m_kakoune_client->state.content, m_kakoune_client->state.default_face, 0.0f, 0.0f);
    m_kakoune_content_view->setWidth(m_width);
    m_content_height = m_height - m_status_bar_view->getCellHeight(ui_options.font);
    m_kakoune_content_view->setHeight(m_content_height);
    m_status_bar_view->render(ui_options.font, m_font_manager, m_kakoune_client->state.mode_line, m_width, m_height);
}

void EditorController::onWindowResize(int width, int height, const UIOptions& ui_options) {
    int rows = (height - m_status_bar_view->getCellHeight(ui_options.font)) / m_kakoune_content_view->getCellHeight(ui_options.font);
    int columns = width / m_kakoune_content_view->getCellWidth(ui_options.font);

    if (rows != m_rows || columns != m_columns) {
        m_kakoune_client->interface->resize(rows, columns);
    }

    m_rows = rows;
    m_columns = columns;
    m_width = width;
    m_height = height;
}

domain::MouseMoveResult EditorController::onMouseMove(float x, float y, const UIOptions* ui_options, bool obscured) {
    if (y >= m_content_height) {
        return domain::MouseMoveResult{domain::Cursor::DEFAULT};
    }

    if (obscured) {
        return domain::MouseMoveResult{domain::Cursor::IBEAM};
    }

    bool is_any_mouse_button_pressed = false;
    for (auto mouse_button : m_mouse_button_pressed) {
        if (mouse_button.second) {
            is_any_mouse_button_pressed = true;
            break;
        }
    }

    if (is_any_mouse_button_pressed) {
        int column = x / ui_options->font->getGlyphMetrics(97).advance;
        int line = y / ui_options->font->getLineHeight();

        m_kakoune_client->interface->moveMouse(line, column);
    }

    return domain::MouseMoveResult{domain::Cursor::IBEAM};
}

void EditorController::onMouseButton(domain::MouseButtonEvent event, const UIOptions* ui_options, bool obscured) {
    int column = event.x / ui_options->font->getGlyphMetrics(97).advance;
    int line = event.y / ui_options->font->getLineHeight();

    if (!obscured && event.action == domain::MouseButtonAction::PRESS) {
        m_kakoune_client->interface->pressMouseButton(event.button, line, column);
        m_mouse_button_pressed[event.button] = true;
    }else if (event.action == domain::MouseButtonAction::RELEASE) {
        if (!obscured) {
            m_kakoune_client->interface->releaseMouseButton(event.button, line, column);
        }

        m_mouse_button_pressed[event.button] = false;
    }
}


int EditorController::width() const {
    return m_width;
}

int EditorController::height() const {
    return m_height;
}

void EditorController::onMouseScroll(int amount, float x, float y, const UIOptions* ui_options) {
    int column = x / ui_options->font->getGlyphMetrics(97).advance;
    int line = y / ui_options->font->getLineHeight();

    m_kakoune_client->interface->scroll(amount, line, column);
}

void EditorController::setClearColor(domain::RGBAColor color) {
    m_set_clear_color(color);
}
