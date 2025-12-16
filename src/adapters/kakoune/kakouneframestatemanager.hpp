#ifndef KAKOUNEFRAMESTATEMANAGER_HPP_INCLUDED
#define KAKOUNEFRAMESTATEMANAGER_HPP_INCLUDED

#include <mutex>
#include <optional>
#include <thread>

#include "kakouneclientprocess.hpp"

struct FrameState
{
    DrawRequestData draw;
    DrawStatusRequestData draw_status;
    std::optional<MenuShowData> menu;
    std::optional<InfoShowData> info_box;
    int menu_selected_index;
};

class KakouneFrameStateManager
{
  public:
    KakouneFrameStateManager(KakouneClientProcess *process);
    ~KakouneFrameStateManager();

    void start();
    void stop();

    std::optional<FrameState> getNextFrameState();

  private:
    void onRequest(const IncomingRequest &request);

    KakouneClientProcess *m_process;

    FrameState m_next_frame_state;
    bool m_next_frame_state_ready;

    bool m_running;

    std::thread m_polling_thread;
    std::mutex m_state_mutex;
};

#endif
