#ifndef KAKOUNEFRAMESTATEMANAGER_HPP_INCLUDED
#define KAKOUNEFRAMESTATEMANAGER_HPP_INCLUDED

#include <functional>
#include <map>
#include <mutex>
#include <optional>
#include <thread>

#include "adapters/kakoune/uioptions.hpp"
#include "kakouneclientprocess.hpp"

struct FrameState
{
    DrawRequestData draw;
    DrawStatusRequestData draw_status;
    std::optional<MenuShowData> menu;
    std::optional<InfoShowData> info_box;
    std::optional<kakoune::UIOptions> ui_options;
    int menu_selected_index;
};

struct FrameEvents
{
    bool menu_select = false;
    bool ui_options_updated = false;
};

class KakouneFrameStateManager
{
  public:
    KakouneFrameStateManager(KakouneClientProcess *process);
    ~KakouneFrameStateManager();

    void start();
    void stop();

    ObserverId onRefresh(std::function<void(bool)> callback);
    void removeObserver(ObserverId id);

    std::optional<FrameState> getNextFrameState();
    FrameEvents popEvents();
    std::optional<std::pair<FrameState, FrameEvents>> getNextFrameStateAndEvents();

  private:
    void notifyRefreshObservers(bool force);

    void onRequest(const IncomingRequest &request);

    KakouneClientProcess *m_process;

    FrameState m_active_frame_state;
    bool m_active_frame_state_ready;
    FrameState m_next_frame_state;
    FrameEvents m_active_frame_events;
    FrameEvents m_next_frame_events;

    bool m_running;

    std::thread m_polling_thread;
    std::mutex m_state_mutex;

    ObserverId m_next_observer_id = 0;
    std::map<ObserverId, std::function<void(bool)>> m_refresh_observers;
};

#endif
