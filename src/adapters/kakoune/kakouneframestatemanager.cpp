#include "kakouneframestatemanager.hpp"
#include "kakouneclientprocess.hpp"
#include <optional>

KakouneFrameStateManager::KakouneFrameStateManager(KakouneClientProcess* process)
    : m_process(process), m_active_frame_state_ready(false), m_running(false) {
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
    case IncomingRequestType::MENU_SHOW: {
        m_next_frame_state.menu = std::get<MenuShowData>(request.data);
        m_next_frame_state.menu_selected_index = -1;
        break;
    }
    case IncomingRequestType::MENU_HIDE: {
        m_next_frame_state.menu = std::nullopt;
        m_next_frame_state.menu_selected_index = -1;
        break;
    }
    case IncomingRequestType::INFO_SHOW: {
        m_next_frame_state.info_box = std::get<InfoShowData>(request.data);
        break;
    }
    case IncomingRequestType::INFO_HIDE: {
        m_next_frame_state.info_box = std::nullopt;
        break;
    }
    case IncomingRequestType::MENU_SELECT: {
        int selected_index = std::get<MenuSelectData>(request.data).selected;
        m_next_frame_state.menu_selected_index = selected_index;
        m_next_frame_events.menu_select = true;
        break;
    }
    case IncomingRequestType::REFRESH: {
        bool force = std::get<RefreshRequestData>(request.data).force;
        m_active_frame_state_ready = true;
        m_active_frame_state = m_next_frame_state;
        m_active_frame_events = m_next_frame_events;
        m_next_frame_events = FrameEvents{};
        notifyRefreshObservers(force);
        break;
    }
    case IncomingRequestType::SET_UI_OPTIONS: {
        m_next_frame_state.ui_options = std::get<UIOptionsData>(request.data).ui_options;
        m_next_frame_events.ui_options_updated = true;
        break;
    }
    }
}

std::optional<FrameState> KakouneFrameStateManager::getNextFrameState() {
    std::lock_guard<std::mutex> lock(m_state_mutex);

    if (!m_active_frame_state_ready) {
        return std::nullopt;
    }

    m_active_frame_state_ready = false;

    return m_active_frame_state;
}

FrameEvents KakouneFrameStateManager::popEvents() {
    std::lock_guard<std::mutex> lock(m_state_mutex);

    FrameEvents events = m_active_frame_events;
    m_active_frame_events = FrameEvents{};

    return events;
}

std::optional<std::pair<FrameState, FrameEvents>> KakouneFrameStateManager::getNextFrameStateAndEvents() {
    std::lock_guard<std::mutex> lock(m_state_mutex);

    if (!m_active_frame_state_ready) {
        return std::nullopt;
    }

    m_active_frame_state_ready = false;

    FrameState state = m_active_frame_state;
    FrameEvents events = m_active_frame_events;

    m_active_frame_events = FrameEvents{};

    return std::make_pair(state, events);
}

ObserverId KakouneFrameStateManager::onRefresh(std::function<void(bool)> callback) {
    ObserverId id = m_next_observer_id++;
    m_refresh_observers[id] = callback;
    return id;
}

void KakouneFrameStateManager::removeObserver(ObserverId id) {
    m_refresh_observers.erase(id);
}

void KakouneFrameStateManager::notifyRefreshObservers(bool force) {
    auto observers_copy = m_refresh_observers;
    for (auto& [id, callback] : observers_copy) {
        callback(force);
    }
}
