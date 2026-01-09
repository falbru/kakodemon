#ifndef KAKOUNEFRAMESTATEMANAGER_HPP_INCLUDED
#define KAKOUNEFRAMESTATEMANAGER_HPP_INCLUDED

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
    bool menu_show = false;
    bool menu_select = false;
    int menu_selected_index = -1;
};

class KakouneFrameStateManager
{
  public:
    KakouneFrameStateManager(KakouneClientProcess *process);
    ~KakouneFrameStateManager();

    void start();
    void stop();

    std::optional<FrameState> getNextFrameState();
    FrameEvents getEvents();
    std::optional<std::pair<FrameState, FrameEvents>> getNextFrameStateAndEvents();

  private:
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
};

#endif
