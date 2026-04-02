#ifndef DOMAIN_COMMANDINTERFACE_HPP_INCLUDED
#define DOMAIN_COMMANDINTERFACE_HPP_INCLUDED

#include "domain/observerlist.hpp"
#include <functional>
#include <string>
#include <vector>

namespace domain
{

struct Command
{
    std::string name;
    std::vector<std::string> args;
};

class CommandInterface
{
  public:
    virtual ~CommandInterface() = default;

    virtual void init() = 0;

    virtual std::vector<Command> getPendingCommands() = 0;

    virtual bool sendCommand(const Command &command) = 0;

    virtual ObserverId onCommandReceived(std::function<void(const Command &)> callback) = 0;
    virtual void removeCommandObserver(ObserverId id) = 0;
};

} // namespace domain

#endif
