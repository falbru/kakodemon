#ifndef KAKOUNE_COLOR_HPP_INCLUDED
#define KAKOUNE_COLOR_HPP_INCLUDED

#include <string>

#include "domain/color.hpp"
#include "nlohmann/json.hpp"

namespace kakoune
{

struct Color
{
    std::string color_string;
};

void to_json(nlohmann::json &j, const Color &c);
void from_json(const nlohmann::json &j, Color &c);

domain::OptionalColor toDomain(Color c);

} // namespace kakoune

#endif
