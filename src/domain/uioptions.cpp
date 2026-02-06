#include "domain/uioptions.hpp"
#include "domain/fontmanager.hpp"

namespace domain
{

bool UIOptions::operator==(const UIOptions &other) const
{
    return font == other.font &&
           font_menu == other.font_menu &&
           font_infobox == other.font_infobox &&
           font_statusbar == other.font_statusbar &&
           font_content == other.font_content &&
           color_border == other.color_border &&
           color_overrides == other.color_overrides;
}

UIOptions getDefaultUIOptions(FontManager *font_manager)
{
    UIOptions ui_options;

    ui_options.font = font_manager->getDefaultFont(14);
    loadBasicGlyphs(ui_options.font);
    ui_options.font_menu = ui_options.font;
    ui_options.font_infobox = ui_options.font;
    ui_options.font_statusbar = ui_options.font;
    ui_options.font_content = ui_options.font;

    return ui_options;
}

} // namespace domain
