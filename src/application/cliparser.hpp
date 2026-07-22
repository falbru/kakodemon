#pragma once

#include "cliconfig.hpp"
#include <functional>
#include <string>

enum class ParseResult
{
    Success,
    ShowHelp,
    ShowVersion,
    SendCommand,
    ShowKakodemonIntegrationScript,
    Error
};

struct CommandRequest
{
    std::string command;
    std::vector<std::string> args;
};

struct ParsedCliArgs
{
    ParseResult result;
    CliConfig config;
    CommandRequest command_request;
    std::string error_message;
};

struct ValidatorDependencies
{
    std::function<bool(const std::string &)> kakouneSessionExists;
};

class CliParser
{
  public:
    CliParser(ValidatorDependencies dependencies);

    ParsedCliArgs parseAndValidate(int argc, char *argv[]);

  private:
    ValidatorDependencies m_validator_depencies;
};

std::string generateRandomSessionId();
