#include "statusstyle.hpp"
#include "spdlog/spdlog.h"

void kakoune::to_json(nlohmann::json &j, const kakoune::StatusStyle &style)
{
    switch(style) {
        case kakoune::StatusStyle::PROMPT:
            j = "prompt";
            break;
        case kakoune::StatusStyle::COMMAND:
            j = "command";
            break;
        case kakoune::StatusStyle::STATUS:
            j = "status";
            break;
        case kakoune::StatusStyle::SEARCH:
            j = "search";
            break;
    }
}

void kakoune::from_json(const nlohmann::json &j, kakoune::StatusStyle &style)
{
    if (j == "prompt") {
        style = kakoune::StatusStyle::PROMPT;
    }else if (j == "command") {
        style = kakoune::StatusStyle::COMMAND;
    }else if (j == "status") {
        style = kakoune::StatusStyle::STATUS;
    }else if (j == "search") {
        style = kakoune::StatusStyle::SEARCH;
    }else if (j.is_string()) {
        spdlog::warn("StatusStyle '{}' not supported", j.get<std::string>());
    }
}