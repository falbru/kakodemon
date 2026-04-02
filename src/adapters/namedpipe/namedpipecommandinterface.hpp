#ifndef NAMEDPIPECOMMANDINTERFACE_HPP_INCLUDED
#define NAMEDPIPECOMMANDINTERFACE_HPP_INCLUDED

#include "domain/observerlist.hpp"
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

class NamedPipeCommandInterface : public domain::CommandInterface
{
  public:
    NamedPipeCommandInterface(const std::string &pipe_id, PipeMode mode);
    ~NamedPipeCommandInterface() override;

    void init() override;

    std::vector<domain::Command> getPendingCommands() override;

    bool sendCommand(const domain::Command &command) override;

    domain::ObserverId onCommandReceived(std::function<void(const domain::Command &)> callback) override;
    void removeCommandObserver(domain::ObserverId id) override;

  private:
    std::string m_pipe_path;
    PipeMode m_mode;

    std::atomic<bool> m_running;
    std::atomic<bool> m_ready;
    std::thread m_read_thread;
    std::mutex m_commands_mutex;
    std::vector<domain::Command> m_pending_commands;

    domain::ObserverList<const domain::Command &> m_command_observers;

    void readLoop();
};

#endif
