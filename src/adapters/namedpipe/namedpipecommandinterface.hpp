#ifndef NAMEDPIPECOMMANDINTERFACE_HPP_INCLUDED
#define NAMEDPIPECOMMANDINTERFACE_HPP_INCLUDED

#include "application/observerid.hpp"
#include "domain/ports/commandinterface.hpp"
#include <atomic>
#include <functional>
#include <map>
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

    std::vector<Command> getPendingCommands() override;

    bool sendCommand(const Command &command) override;

    ObserverId onCommandReceived(std::function<void(const Command &)> callback) override;
    void removeCommandObserver(ObserverId id) override;

  private:
    std::string m_pipe_path;
    PipeMode m_mode;

    std::atomic<bool> m_running;
    std::atomic<bool> m_ready;
    std::thread m_read_thread;
    std::mutex m_commands_mutex;
    std::vector<Command> m_pending_commands;

    ObserverId m_next_observer_id = 0;
    std::map<ObserverId, std::function<void(const Command &)>> m_command_observers;

    void readLoop();
    void notifyCommandObservers(const Command &command);
};

#endif
