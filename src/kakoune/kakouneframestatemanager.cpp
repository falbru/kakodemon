#include "kakouneframestatemanager.hpp"
#include "kakoune/kakouneclientprocess.hpp"

KakouneFrameStateManager::KakouneFrameStateManager(std::shared_ptr<KakouneClientProcess> process)
    : m_process(process), m_next_frame_state_ready(false), m_running(false) {
    m_process->setRequestCallback([this](const IncomingRequest& request) {
        onRequest(request);
    });
}

KakouneFrameStateManager::~KakouneFrameStateManager()
{
    stop();
}

void KakouneFrameStateManager::start()
{
    if (m_running) return;

    m_running = true;
    m_polling_thread = std::thread([this]() {
        while (m_running) {
            m_process->pollForRequests();
        }
    });
}

void KakouneFrameStateManager::stop()
{
    if (!m_running) return;

    m_running = false;
    if (m_polling_thread.joinable()) {
        m_polling_thread.join();
    }
}

void KakouneFrameStateManager::onRequest(const IncomingRequest& request)
{
    std::lock_guard<std::mutex> lock(m_state_mutex);

    switch (request.type)
    {
    case IncomingRequestType::DRAW: {
        m_next_frame_state.draw = std::get<DrawRequestData>(request.data);
        break;
    }
    case IncomingRequestType::DRAW_STATUS: {
        m_next_frame_state.draw_status = std::get<DrawStatusRequestData>(request.data);
        break;
    }
    case IncomingRequestType::REFRESH: {
        m_next_frame_state_ready = true;
    }
    default:
        break;
    }
}

std::optional<FrameState> KakouneFrameStateManager::getNextFrameState() {
    std::lock_guard<std::mutex> lock(m_state_mutex);

    if (!m_next_frame_state_ready) return std::nullopt;

    m_next_frame_state_ready = false;

    return m_next_frame_state;
}
