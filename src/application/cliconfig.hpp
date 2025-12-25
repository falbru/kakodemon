#pragma once

#include <optional>
#include <string>

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
};
