#include "editorcontroller.hpp"
#include "kakoune/kakouneclientprocess.hpp"

EditorController::EditorController()
{
}

void EditorController::init(std::shared_ptr<KakouneClient> kakoune_client,
                            std::shared_ptr<KakouneClientProcess> kakoune_process,
                            std::shared_ptr<KakouneContentView> kakoune_content_view)
{
    m_kakoune_client = kakoune_client;
    m_kakoune_process = kakoune_process;
    m_kakoune_content_view = kakoune_content_view;

    m_frame_state_manager = std::make_unique<KakouneFrameStateManager>(m_kakoune_process);
}

void EditorController::update()
{
    m_frame_state_manager->update();

    auto frame_state = m_frame_state_manager->getNextFrameState();
    if (frame_state.has_value()) {
        m_kakoune_client->window_content = frame_state.value().draw.lines;
    }

    m_kakoune_content_view->render(m_kakoune_client->window_content, 0.0f, 0.0f);
}

void EditorController::onWindowResize(int width, int height) {
    int rows = height / m_kakoune_content_view->getCharHeight();
    int columns = width / m_kakoune_content_view->getCharWidth();

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
}
