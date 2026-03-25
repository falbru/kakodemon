#ifndef STATUSSTYLE_HPP_INCLUDED
#define STATUSSTYLE_HPP_INCLUDED

#include "nlohmann/json.hpp"

namespace kakoune
{
enum class StatusStyle
{
    PROMPT,
    COMMAND,
    STATUS,
    SEARCH
};

void to_json(nlohmann::json &j, const StatusStyle &style);
void from_json(const nlohmann::json &j, StatusStyle &style);

}

#endif
