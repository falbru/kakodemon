#pragma once

#include <optional>
#include <string>
#include <vector>

enum class SessionType
{
    Local,
    Remote
};

struct CliConfig
{
    SessionType session_type = SessionType::Local;
    std::string session_id;
    std::optional<std::string> startup_command = std::nullopt;
    std::vector<std::string> file_arguments;
};
