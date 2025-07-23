#include "kakouneframestatemanager.hpp"

KakouneFrameStateManager::KakouneFrameStateManager(std::shared_ptr<KakouneClientProcess> process)
    : m_process(process), m_next_frame_state_ready(false) {

}

void KakouneFrameStateManager::update() {
    m_process->pollForRequests();

    std::optional<IncomingRequest> request = m_process->getNextRequest();

    while (request.has_value()) {
        IncomingRequest request_value = request.value();
        switch (request_value.type)
        {
        case IncomingRequestType::DRAW: {
            m_next_frame_state.draw = std::get<DrawRequestData>(request_value.data);
            break;
        }
        case IncomingRequestType::REFRESH: {
            m_next_frame_state_ready = true;
        }
        default:
            break;
        }

        request = m_process->getNextRequest();
    }
}

std::optional<FrameState> KakouneFrameStateManager::getNextFrameState() {
    if (!m_next_frame_state_ready) return std::nullopt;

    m_next_frame_state_ready = false;

    return m_next_frame_state;
}
