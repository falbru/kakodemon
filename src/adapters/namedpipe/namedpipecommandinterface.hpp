#ifndef NAMEDPIPECOMMANDINTERFACE_HPP_INCLUDED
#define NAMEDPIPECOMMANDINTERFACE_HPP_INCLUDED

#include "domain/ports/commandinterface.hpp"
#include <atomic>
#include <functional>
#include <mutex>
#include <thread>

enum class PipeMode
{
    Receive,
    Send,
    Both
};

class NamedPipeCommandInterface : public CommandInterface
{
  public:
    NamedPipeCommandInterface(const std::string &pipe_id, PipeMode mode);
    ~NamedPipeCommandInterface() override;

    void init() override;
    void setWakeEventLoopCallback(std::function<void()> callback) override;

    std::vector<Command> getPendingCommands() override;

    bool sendCommand(const Command &command) override;

  private:
    std::string m_pipe_path;
    PipeMode m_mode;

    std::atomic<bool> m_running;
    std::atomic<bool> m_ready;
    std::thread m_read_thread;
    std::mutex m_commands_mutex;
    std::vector<Command> m_pending_commands;
    std::function<void()> m_wake_event_loop_callback;

    void readLoop();
};

#endif
