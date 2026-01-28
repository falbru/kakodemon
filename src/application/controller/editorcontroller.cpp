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

void EditorController::init(std::vector<std::unique_ptr<KakouneClient>>* kakoune_clients,
                            KakouneClient** focused_client,
                            LayoutController* layout_controller,
                            KakouneContentView* kakoune_content_view,
                            StatusBarView* status_bar_view,
                            domain::FontManager* font_manager,
                            std::function<void(domain::RGBAColor)> set_clear_color,
                            MenuController* menu_controller)
{
    m_kakoune_clients = kakoune_clients;
    m_focused_client = focused_client;
    m_layout_controller = layout_controller;
    m_kakoune_content_view = kakoune_content_view;
    m_status_bar_view = status_bar_view;
    m_font_manager = font_manager;
    m_set_clear_color = set_clear_color;
    m_menu_controller = menu_controller;
}

bool EditorController::update(domain::UIOptions& ui_options)
{
    bool state_updated = false;

    for (auto& client : *m_kakoune_clients) {
        auto result = client->interface->getNextKakouneStateAndEvents();
        if (result.has_value()) {
            client->state = result->first;
            domain::FrameEvents events = result->second;
            state_updated = true;

            if (events.menu_select && client->state.menu.has_value() && client->state.menu->hasItems()) {
                m_menu_controller->ensureItemVisible(client->state.menu->getItems().selected_index);
            }
        }
    }

    if (state_updated && !m_kakoune_clients->empty()) {
        auto& first_client = m_kakoune_clients->front();
        setClearColor(first_client->state.default_face.getBg());

        domain::UIOptions new_ui_options = first_client->interface->getUIOptions(m_font_manager);
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
    for (const auto& layout : m_layout_controller->getLayouts()) {
        auto* client = layout.client;
        const auto& bounds = layout.bounds;

        RenderContext render_context = {
            m_font_manager,
            client->state.default_face,
            ui_options,
            bounds.width,
            bounds.height
        };

        m_kakoune_content_view->render(render_context, client->state.content, client->state.default_face, bounds);
        m_status_bar_view->render(render_context, client->status_line_state, client->state.mode_line, client->state.cursor_position, bounds);
    }
}

void EditorController::onWindowResize(int width, int height, const domain::UIOptions& ui_options) {
    m_width = width;
    m_height = height;

    m_layout_controller->arrange(static_cast<float>(width), static_cast<float>(height));

    float cell_width = m_kakoune_content_view->getCellWidth(ui_options.font_content);
    float cell_height = m_kakoune_content_view->getCellHeight(ui_options.font_content);
    float status_bar_height = m_status_bar_view->height(ui_options.font_statusbar);

    for (const auto& layout : m_layout_controller->getLayouts()) {
        int rows = static_cast<int>((layout.bounds.height - status_bar_height) / cell_height);
        int columns = static_cast<int>(layout.bounds.width / cell_width);
        layout.client->interface->resize(rows, columns);
    }

    if (!m_layout_controller->getLayouts().empty()) {
        const auto& first_layout = m_layout_controller->getLayouts().front();
        m_rows = static_cast<int>((first_layout.bounds.height - status_bar_height) / cell_height);
    }
}

domain::MouseMoveResult EditorController::onMouseMove(float x, float y, const domain::UIOptions* ui_options, bool obscured) {
    ClientLayout* hover_layout = m_layout_controller->findLayoutAt(x, y);
    if (!hover_layout) {
        return domain::MouseMoveResult{domain::Cursor::DEFAULT};
    }

    float status_bar_height = m_status_bar_view->height(ui_options->font_statusbar);
    float content_height = hover_layout->bounds.height - status_bar_height;
    float relative_y = y - hover_layout->bounds.y;

    if (relative_y >= content_height) {
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
        ClientLayout* focused_layout = m_layout_controller->findLayoutForClient(*m_focused_client);
        if (focused_layout) {
            float relative_x = x - focused_layout->bounds.x;
            float focused_relative_y = y - focused_layout->bounds.y;
            domain::Coord coord = m_kakoune_content_view->pixelToCoord(ui_options->font_content, relative_x, focused_relative_y);
            (*m_focused_client)->interface->moveMouse(coord.line, coord.column);
            m_active_mouse_client = *m_focused_client;
        }
    }

    return domain::MouseMoveResult{domain::Cursor::IBEAM};
}

void EditorController::onMouseButton(domain::MouseButtonEvent event, const domain::UIOptions* ui_options, bool obscured) {
    ClientLayout* focused_layout = m_layout_controller->findLayoutForClient(*m_focused_client);
    if (!focused_layout) {
        return;
    }

    float relative_x = event.x - focused_layout->bounds.x;
    float relative_y = event.y - focused_layout->bounds.y;
    domain::Coord coord = m_kakoune_content_view->pixelToCoord(ui_options->font_content, relative_x, relative_y);

    if (!obscured && event.action == domain::MouseButtonAction::PRESS) {
        (*m_focused_client)->interface->pressMouseButton(event.button, coord.line, coord.column);
        m_mouse_button_pressed[event.button] = true;
    } else if (event.action == domain::MouseButtonAction::RELEASE) {
        if (!obscured && m_active_mouse_client) {
            m_active_mouse_client->interface->releaseMouseButton(event.button, coord.line, coord.column);
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
    ClientLayout* focused_layout = m_layout_controller->findLayoutForClient(*m_focused_client);
    if (!focused_layout) {
        return;
    }

    float relative_x = x - focused_layout->bounds.x;
    float relative_y = y - focused_layout->bounds.y;
    domain::Coord coord = m_kakoune_content_view->pixelToCoord(ui_options->font_content, relative_x, relative_y);

    (*m_focused_client)->interface->scroll(amount, coord.line, coord.column);
}

void EditorController::setClearColor(domain::RGBAColor color) {
    m_set_clear_color(color);
}
