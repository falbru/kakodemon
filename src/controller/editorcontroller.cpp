#include "editorcontroller.hpp"

EditorController::EditorController()
{
}

void EditorController::init(std::shared_ptr<KakouneClient> kakoune_client,
                            std::shared_ptr<KakouneClientProcess> kakoune_process,
                            std::shared_ptr<KakouneContentView> kakoune_content_view)
{
    m_kakoune_client = kakoune_client;
    m_kakoune_content_view = kakoune_content_view;

    m_frame_state_manager = std::make_unique<KakouneFrameStateManager>(kakoune_process);
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
