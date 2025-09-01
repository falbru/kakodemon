#include "editorcontroller.hpp"
#include "kakoune/kakouneclientprocess.hpp"
#include "view/statusbar.hpp"
#include <memory>

EditorController::EditorController()
{
}

void EditorController::init(std::shared_ptr<KakouneClient> kakoune_client,
                            std::shared_ptr<KakouneClientProcess> kakoune_process,
                            std::shared_ptr<KakouneContentView> kakoune_content_view,
                            std::shared_ptr<StatusBarView> status_bar_view,
                            std::shared_ptr<MenuView> menu_view
                            )
{
    m_kakoune_client = kakoune_client;
    m_kakoune_process = kakoune_process;
    m_kakoune_content_view = kakoune_content_view;
    m_status_bar_view = status_bar_view;
    m_menu_view = menu_view;

    m_frame_state_manager = std::make_unique<KakouneFrameStateManager>(m_kakoune_process);
    m_frame_state_manager->start();
}

void EditorController::update()
{
    auto frame_state = m_frame_state_manager->getNextFrameState();
    if (frame_state.has_value()) {
        m_kakoune_client->window_content = frame_state.value().draw.lines;
        m_kakoune_client->window_default_face = frame_state.value().draw.default_face;
        m_kakoune_client->status_line = frame_state.value().draw_status.status_line;
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
    }

    m_kakoune_content_view->render(m_kakoune_client->window_content, m_kakoune_client->window_default_face, 0.0f, 0.0f);
    m_status_bar_view->render(m_kakoune_client->status_line, m_kakoune_client->mode_line, m_kakoune_client->status_default_face, m_width, m_height);
    m_menu_view->render(*m_kakoune_client,
                           m_width,
                           m_height);
}

void EditorController::onWindowResize(int width, int height) {
    int rows = (height - m_status_bar_view->getCellHeight()) / m_kakoune_content_view->getCellHeight();
    int columns = width / m_kakoune_content_view->getCellWidth();

    if (rows != m_rows || columns != m_columns) {
        m_kakoune_process->sendRequest(OutgoingRequest{
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
