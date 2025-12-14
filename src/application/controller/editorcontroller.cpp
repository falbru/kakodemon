#include "editorcontroller.hpp"
#include "application/model/uioptions.hpp"
#include "domain/color.hpp"
#include "domain/mouse.hpp"
#include "domain/editor.hpp"
#include "kakoune/kakouneclientprocess.hpp"
#include "application/view/statusbar.hpp"
#include "spdlog/spdlog.h"
#include <memory>

EditorController::EditorController()
{
}

void EditorController::init(KakouneClient* kakoune_client,
                            KakouneContentView* kakoune_content_view,
                            StatusBarView* status_bar_view,
                            std::function<void(domain::Color)> set_clear_color)
{
    m_set_clear_color = set_clear_color;

    m_kakoune_client = kakoune_client;

    m_kakoune_content_view = kakoune_content_view;
    m_status_bar_view = status_bar_view;

    m_frame_state_manager = std::make_unique<KakouneFrameStateManager>(m_kakoune_client->process.get());
    m_frame_state_manager->start();
}

void EditorController::update(const UIOptions& ui_options)
{
    auto frame_state = m_frame_state_manager->getNextFrameState();
    if (frame_state.has_value()) {
        m_kakoune_client->window_content = frame_state.value().draw.lines;
        m_kakoune_client->window_default_face = frame_state.value().draw.default_face;
        m_kakoune_client->status_line = domain::StatusLine{frame_state.value().draw_status.status_line_prompt , frame_state.value().draw_status.status_line_content};
        m_kakoune_client->cursor_pos = domain::StatusLinePosition{frame_state.value().draw_status.cursor_pos};
        m_kakoune_client->mode_line = frame_state.value().draw_status.mode_line;
        m_kakoune_client->status_default_face = frame_state.value().draw_status.default_face;
        m_kakoune_client->menu_selected_index = frame_state.value().menu_selected_index;
        if (frame_state.value().menu.has_value()) {
            auto menu_request = frame_state.value().menu.value();

            m_kakoune_client->menu_visible = true;
            m_kakoune_client->menu_items = menu_request.items;
            m_kakoune_client->menu_anchor = menu_request.anchor;
            m_kakoune_client->menu_face = menu_request.face;
            m_kakoune_client->menu_selected_face = menu_request.selected_face;
            m_kakoune_client->menu_style = menu_request.style;
        }else {
            m_kakoune_client->menu_visible = false;
        }
        if (frame_state.value().info_box.has_value()) {
            auto info_box_request = frame_state.value().info_box.value();

            m_kakoune_client->info_box_visible = true;
            m_kakoune_client->info_box_title = info_box_request.title;
            m_kakoune_client->info_box_content = info_box_request.content;
            m_kakoune_client->info_box_anchor = info_box_request.anchor;
            m_kakoune_client->info_box_face = info_box_request.face;
            m_kakoune_client->info_box_style = info_box_request.style;
        }else {
            m_kakoune_client->info_box_visible = false;
        }

        setClearColor(m_kakoune_client->window_default_face.bg.toCoreColor(std::nullopt, false));
    }

    m_kakoune_content_view->render(ui_options.font.get(), m_kakoune_client->window_content, m_kakoune_client->window_default_face, 0.0f, 0.0f);
    m_kakoune_content_view->setWidth(m_width);
    m_content_height = m_height - m_status_bar_view->getCellHeight(ui_options.font.get());
    m_kakoune_content_view->setHeight(m_content_height);
    m_status_bar_view->render(ui_options.font.get(), m_kakoune_client->mode_line, m_kakoune_client->status_default_face, m_width, m_height);
}

void EditorController::onWindowResize(int width, int height, const UIOptions& ui_options) {
    int rows = (height - m_status_bar_view->getCellHeight(ui_options.font.get())) / m_kakoune_content_view->getCellHeight(ui_options.font.get());
    int columns = width / m_kakoune_content_view->getCellWidth(ui_options.font.get());

    if (rows != m_rows || columns != m_columns) {
        m_kakoune_client->process->sendRequest(OutgoingRequest{
            OutgoingRequestType::RESIZE,
            ResizeRequestData{
                rows,
                columns
            }
        });
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
        int column = x / ui_options->font.get()->getGlyphMetrics(97).width();
        int line = y / ui_options->font.get()->getLineHeight();

        m_kakoune_client->process->sendRequest(OutgoingRequest{
            OutgoingRequestType::MOUSE_MOVE,
            MouseMoveRequestData{
                line,
                column
            }
        });
    }

    return domain::MouseMoveResult{domain::Cursor::IBEAM};
}

void EditorController::onMouseButton(domain::MouseButtonEvent event, const UIOptions* ui_options, bool obscured) {
    int column = event.x / ui_options->font.get()->getGlyphMetrics(97).width();
    int line = event.y / ui_options->font.get()->getLineHeight();

    std::string button;
    switch(event.button) {
        case domain::MouseButton::LEFT:
            button = "left";
            break;
        case domain::MouseButton::MIDDLE:
            button = "middle";
            break;
        case domain::MouseButton::RIGHT:
            button = "right";
            break;
    }

    if (!obscured && event.action == domain::MouseButtonAction::PRESS) {
        m_kakoune_client->process->sendRequest(OutgoingRequest{
            OutgoingRequestType::MOUSE_PRESS,
            MousePressRequestData{
                button,
                line,
                column
            }
        });

        m_mouse_button_pressed[event.button] = true;
    }else if (event.action == domain::MouseButtonAction::RELEASE) {
        if (!obscured) {
            m_kakoune_client->process->sendRequest(OutgoingRequest{
                OutgoingRequestType::MOUSE_RELEASE,
                MouseReleaseRequestData{
                    button,
                    line,
                    column
                }
            });
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

void EditorController::setClearColor(domain::Color color) {
    m_set_clear_color(color);
}
