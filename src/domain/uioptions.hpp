#ifndef MODEL_UIOPTIONS_HPP_INCLUDED
#define MODEL_UIOPTIONS_HPP_INCLUDED

#include "domain/color.hpp"
#include "domain/ports/font.hpp"
#include <unordered_map>

namespace domain
{

class FontManager;

struct UIOptions
{
    Font *font = nullptr;
    Font *font_menu = nullptr;
    Font *font_infobox = nullptr;
    Font *font_statusbar = nullptr;
    Font *font_content = nullptr;
    OptionalColor color_border;
    std::unordered_map<FixedColor, RGBAColor> color_overrides;

    bool operator==(const UIOptions &other) const;
};

UIOptions getDefaultUIOptions(FontManager *font_manager);

} // namespace domain

#endif
