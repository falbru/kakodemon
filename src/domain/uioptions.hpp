#ifndef MODEL_UIOPTIONS_HPP_INCLUDED
#define MODEL_UIOPTIONS_HPP_INCLUDED

#include "domain/color.hpp"
#include "domain/ports/font.hpp"
#include <unordered_map>

namespace domain
{

struct UIOptions
{
    Font *font = nullptr;
    OptionalColor color_border;
    std::unordered_map<FixedColor, RGBAColor> color_overrides;
};

} // namespace domain

#endif
