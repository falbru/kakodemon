#ifndef MENUSTYLE_HPP_INCLUDED
#define MENUSTYLE_HPP_INCLUDED

#include "nlohmann/json.hpp"

namespace kakoune
{
enum class MenuStyle
{
    PROMPT,
    INLINE,
    SEARCH
};

void to_json(nlohmann::json &j, const MenuStyle &style);
void from_json(const nlohmann::json &j, MenuStyle &style);
} // namespace kakoune

#endif // menustyle_hpp_INCLUDED
