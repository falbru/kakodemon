#ifndef KAKOUNE_UIOPTIONS_HPP_INCLUDED
#define KAKOUNE_UIOPTIONS_HPP_INCLUDED

#include "nlohmann/json.hpp"
#include <optional>
#include <string>

namespace kakoune
{

struct UIOptions
{
    std::optional<std::string> font;
};

void to_json(nlohmann::json &j, const UIOptions &ui_options);
void from_json(const nlohmann::json &j, UIOptions &ui_options);

} // namespace kakoune

#endif
