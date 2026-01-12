#ifndef KAKOUNE_UIOPTIONS_HPP_INCLUDED
#define KAKOUNE_UIOPTIONS_HPP_INCLUDED

#include "adapters/kakoune/color.hpp"
#include "nlohmann/json.hpp"
#include <optional>
#include <string>

namespace kakoune
{

struct UIOptions
{
    std::optional<std::string> font;
    std::optional<Color> color_border;
    std::optional<Color> color_black;
    std::optional<Color> color_red;
    std::optional<Color> color_green;
    std::optional<Color> color_yellow;
    std::optional<Color> color_blue;
    std::optional<Color> color_magenta;
    std::optional<Color> color_cyan;
    std::optional<Color> color_white;
    std::optional<Color> color_bright_black;
    std::optional<Color> color_bright_red;
    std::optional<Color> color_bright_green;
    std::optional<Color> color_bright_yellow;
    std::optional<Color> color_bright_blue;
    std::optional<Color> color_bright_magenta;
    std::optional<Color> color_bright_cyan;
    std::optional<Color> color_bright_white;
};

void to_json(nlohmann::json &j, const UIOptions &ui_options);
void from_json(const nlohmann::json &j, UIOptions &ui_options);

} // namespace kakoune

#endif
