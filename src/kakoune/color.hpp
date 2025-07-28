#ifndef KAKOUNE_COLOR_HPP_INCLUDED
#define KAKOUNE_COLOR_HPP_INCLUDED

#include <string>

#include "core/color.hpp"
#include "nlohmann/json.hpp"

namespace kakoune
{

struct Color
{
    std::string color_string;

    core::Color toCoreColor(std::optional<Color> default_color, bool fg) const;
};

void to_json(nlohmann::json &j, const Color &c);
void from_json(const nlohmann::json &j, Color &c);

} // namespace kakoune

#endif
