#ifndef KAKOUNEFRAMESTATEMANAGER_HPP_INCLUDED
#define KAKOUNEFRAMESTATEMANAGER_HPP_INCLUDED

#include <memory>
#include <mutex>
#include <optional>
#include <thread>

#include "kakouneclientprocess.hpp"

struct FrameState
{
    DrawRequestData draw;
};

class KakouneFrameStateManager
{
  public:
    KakouneFrameStateManager(std::shared_ptr<KakouneClientProcess> process);
    ~KakouneFrameStateManager();

    void start();
    void stop();

    std::optional<FrameState> getNextFrameState();

  private:
    void onRequest(const IncomingRequest &request);

    std::shared_ptr<KakouneClientProcess> m_process;

    FrameState m_next_frame_state;
    bool m_next_frame_state_ready;

    bool m_running;

    std::thread m_polling_thread;
    std::mutex m_state_mutex;
};

#endif
