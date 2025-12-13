#include "menustyle.hpp"
#include "spdlog/spdlog.h"

void kakoune::to_json(nlohmann::json &j, const kakoune::MenuStyle &style)
{
    switch(style) {
        case kakoune::MenuStyle::PROMPT:
            j = "prompt";
            break;
        case kakoune::MenuStyle::INLINE:
            j = "inline";
            break;
        case kakoune::MenuStyle::SEARCH:
            j = "search";
            break;
    }
}

void kakoune::from_json(const nlohmann::json &j, kakoune::MenuStyle &style)
{
    if (j == "prompt") {
        style = kakoune::MenuStyle::PROMPT;
    }else if (j == "inline") {
        style = kakoune::MenuStyle::INLINE;
    }else if (j == "search") {
        style = kakoune::MenuStyle::SEARCH;
    }else if (j.is_string()){
        spdlog::warn("Menustyle '{}' not supported", j.get<std::string>());
    }
}
