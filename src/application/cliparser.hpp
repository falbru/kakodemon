#pragma once

#include "cliconfig.hpp"
#include <string>

enum class ParseResult
{
    Success,
    ShowHelp,
    ShowVersion,
    SendCommand,
    Error
};

struct CommandRequest
{
    std::string pipe_id;
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

ParsedCliArgs parseCliArgs(int argc, char *argv[]);

std::string generate_random_session_id();
