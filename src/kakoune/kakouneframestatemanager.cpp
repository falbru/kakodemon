#include "kakouneframestatemanager.hpp"

KakouneFrameStateManager::KakouneFrameStateManager(std::shared_ptr<KakouneClientProcess> process)
    : m_process(process), m_next_frame_state_ready(false) {
    m_process->setRequestCallback([this](const IncomingRequest& request) {
        onRequest(request);
    });
}

void KakouneFrameStateManager::onRequest(const IncomingRequest& request)
{
    switch (request.type)
    {
    case IncomingRequestType::DRAW: {
        m_next_frame_state.draw = std::get<DrawRequestData>(request.data);
        break;
    }
    case IncomingRequestType::REFRESH: {
        m_next_frame_state_ready = true;
    }
    default:
        break;
    }
}

void KakouneFrameStateManager::update() {
    m_process->pollForRequests();
}

std::optional<FrameState> KakouneFrameStateManager::getNextFrameState() {
    if (!m_next_frame_state_ready) return std::nullopt;

    m_next_frame_state_ready = false;

    return m_next_frame_state;
}
