#pragma once

#include "cliconfig.hpp"
#include <string>

enum class ParseResult
{
    Success,
    ShowHelp,
    ShowVersion,
    Error
};

struct ParsedCliArgs
{
    ParseResult result;
    CliConfig config;
    std::string error_message;
};

ParsedCliArgs parseCliArgs(int argc, char *argv[]);

std::string generate_random_session_id();
