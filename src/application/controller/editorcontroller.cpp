#include "editorcontroller.hpp"
#include "application/controller/menucontroller.hpp"
#include "application/view/rendercontext.hpp"
#include "domain/uioptions.hpp"
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
                            std::function<void(domain::RGBAColor)> set_clear_color,
                            MenuController* menu_controller)
{
    m_kakoune_client = kakoune_client;
    m_kakoune_content_view = kakoune_content_view;
    m_status_bar_view = status_bar_view;
    m_font_manager = font_manager;
    m_set_clear_color = set_clear_color;
    m_menu_controller = menu_controller;
}

bool EditorController::update(domain::UIOptions& ui_options)
{
    bool state_updated = false;
    auto result = m_kakoune_client->interface->getNextKakouneStateAndEvents();
    if (result.has_value()) {
        m_kakoune_client->state = result->first;
        domain::FrameEvents events = result->second;

        setClearColor(m_kakoune_client->state.default_face.getBg());
        state_updated = true;

        if (events.menu_select && m_kakoune_client->state.menu.has_value() && m_kakoune_client->state.menu->hasItems()) {
            m_menu_controller->ensureItemVisible(m_kakoune_client->state.menu->getItems().selected_index);
        }

        domain::UIOptions new_ui_options = m_kakoune_client->interface->getUIOptions(m_font_manager);
        bool fonts_changed = new_ui_options.font_content != ui_options.font_content ||
                             new_ui_options.font_statusbar != ui_options.font_statusbar;
        ui_options = new_ui_options;

        if (fonts_changed && m_width > 0 && m_height > 0) {
            onWindowResize(m_width, m_height, ui_options);
        }
    }

    return state_updated;
}

void EditorController::render(const domain::UIOptions& ui_options)
{
    RenderContext render_context = {
        m_font_manager,
        m_kakoune_client->state.default_face,
        ui_options,
        static_cast<float>(m_width),
        static_cast<float>(m_height)
    };

    m_kakoune_content_view->render(render_context, m_kakoune_client->state.content, m_kakoune_client->state.default_face, 0.0f, 0.0f);
    m_kakoune_content_view->setWidth(m_width);
    m_content_height = m_rows * m_kakoune_content_view->getCellHeight(ui_options.font_content);
    m_kakoune_content_view->setHeight(m_content_height);
    m_status_bar_view->render(render_context, m_kakoune_client->state.mode_line);
}

void EditorController::onWindowResize(int width, int height, const domain::UIOptions& ui_options) {
    int rows = (height - m_status_bar_view->height(ui_options.font_statusbar)) / m_kakoune_content_view->getCellHeight(ui_options.font_content);
    int columns = width / m_kakoune_content_view->getCellWidth(ui_options.font_content);

    if (rows != m_rows || columns != m_columns) {
        m_kakoune_client->interface->resize(rows, columns);
    }

    m_rows = rows;
    m_columns = columns;
    m_width = width;
    m_height = height;
}

domain::MouseMoveResult EditorController::onMouseMove(float x, float y, const domain::UIOptions* ui_options, bool obscured) {
    if (y >= m_content_height) {
        return domain::MouseMoveResult{domain::Cursor::DEFAULT};
    }

    if (obscured) {
        return domain::MouseMoveResult{domain::Cursor::IBEAM};
    }

    bool is_any_mouse_button_pressed = false;
    for (const auto& mouse_button : m_mouse_button_pressed) {
        if (mouse_button.second) {
            is_any_mouse_button_pressed = true;
            break;
        }
    }

    if (is_any_mouse_button_pressed) {
        int column = x / ui_options->font_content->getGlyphMetrics(97).advance;
        int line = y / ui_options->font_content->getLineHeight();

        m_kakoune_client->interface->moveMouse(line, column);
    }

    return domain::MouseMoveResult{domain::Cursor::IBEAM};
}

void EditorController::onMouseButton(domain::MouseButtonEvent event, const domain::UIOptions* ui_options, bool obscured) {
    int column = event.x / ui_options->font_content->getGlyphMetrics(97).advance;
    int line = event.y / ui_options->font_content->getLineHeight();

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

void EditorController::onMouseScroll(int amount, float x, float y, const domain::UIOptions* ui_options) {
    int column = x / ui_options->font_content->getGlyphMetrics(97).advance;
    int line = y / ui_options->font_content->getLineHeight();

    m_kakoune_client->interface->scroll(amount, line, column);
}

void EditorController::setClearColor(domain::RGBAColor color) {
    m_set_clear_color(color);
}
