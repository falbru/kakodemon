#ifndef KAKOUNE_INFOSTYLE_HPP_INCLUDED
#define KAKOUNE_INFOSTYLE_HPP_INCLUDED

#include "domain/infobox.hpp"
#include "nlohmann/json.hpp"

namespace kakoune
{
enum class InfoStyle
{
    PROMPT,
    INLINE,
    INLINE_ABOVE,
    INLINE_BELOW,
    MENUDOC,
    MODAL,
};

void to_json(nlohmann::json &j, const InfoStyle &style);
void from_json(const nlohmann::json &j, InfoStyle &style);

domain::InfoStyle toDomain(InfoStyle style);

} // namespace kakoune

#endif
