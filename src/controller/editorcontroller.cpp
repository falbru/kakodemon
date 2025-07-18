#include "editorcontroller.hpp"

#include <memory>

EditorController::EditorController()
{
}

EditorController::~EditorController()
{
}

void EditorController::init(std::shared_ptr<KakouneClient> kakoune_client,
                            std::shared_ptr<KakouneContentView> kakoune_content_view)
{
    m_kakoune_client = kakoune_client;
    m_kakoune_content_view = kakoune_content_view;
}

void EditorController::update()
{
    m_kakoune_client->pollForRequests();

    std::optional<KakouneClientRequest> request = m_kakoune_client->getNextRequest();

    if (request.has_value())
    {
        KakouneClientRequest request_value = request.value();
        switch (request_value.type)
        {
        case KakouneRequestType::DRAW: {
            KakouneDrawRequestData &draw_data = std::get<KakouneDrawRequestData>(request_value.data);
            m_kakoune_client->setWindowContent(draw_data.lines);
            break;
        }
        default:
            break;
        }
    }

    m_kakoune_content_view->render(m_kakoune_client->getWindowContent(), 0.0f, 0.0f);
}
