#include "kakoune/infostyle.hpp"
#include "spdlog/spdlog.h"

void kakoune::to_json(nlohmann::json &j, const kakoune::InfoStyle &style)
{
    switch(style) {
        case kakoune::InfoStyle::PROMPT:
            j = "prompt";
            break;
        case kakoune::InfoStyle::INLINE:
            j = "inline";
            break;
        case kakoune::InfoStyle::INLINE_ABOVE:
            j = "inlineAbove";
            break;
        case kakoune::InfoStyle::INLINE_BELOW:
            j = "inlineBelow";
            break;
        case kakoune::InfoStyle::MENUDOC:
            j = "menuDoc";
            break;
        case kakoune::InfoStyle::MODAL:
            j = "modal";
            break;
    }
}

void kakoune::from_json(const nlohmann::json &j, kakoune::InfoStyle &style)
{
    if (j == "prompt") {
        style = kakoune::InfoStyle::PROMPT;
    }else if (j == "inline") {
        style = kakoune::InfoStyle::INLINE;
    }else if (j == "inlineAbove") {
        style = kakoune::InfoStyle::INLINE_ABOVE;
    }else if (j == "inlineBelow") {
        style = kakoune::InfoStyle::INLINE_BELOW;
    }else if (j == "menuDoc") {
        style = kakoune::InfoStyle::MENUDOC;
    }else if (j == "modal") {
        style = kakoune::InfoStyle::MODAL;
    }else if (j.is_string()) {
        spdlog::warn("InfoStyle '{}' not supported", j.get<std::string>());
    }
}
