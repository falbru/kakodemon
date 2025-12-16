#include "infostyle.hpp"
#include "domain/infobox.hpp"
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

domain::InfoStyle kakoune::toDomain(kakoune::InfoStyle style) {
    switch(style) {
        case kakoune::InfoStyle::PROMPT:
            return domain::InfoStyle::PROMPT;
        case kakoune::InfoStyle::INLINE:
            return domain::InfoStyle::INLINE;
        case kakoune::InfoStyle::INLINE_ABOVE:
            return domain::InfoStyle::INLINE_ABOVE;
        case kakoune::InfoStyle::INLINE_BELOW:
            return domain::InfoStyle::INLINE_BELOW;
        case kakoune::InfoStyle::MENUDOC:
            return domain::InfoStyle::MENUDOC;
        case kakoune::InfoStyle::MODAL:
            return domain::InfoStyle::MODAL;
        default: {
            spdlog::warn("Can't convert Kakoune InfoStyle {} to domain", (int)style);
            return domain::InfoStyle::PROMPT;
        }
    }
}
