#include "application/view/rendercontext.hpp"

domain::TextRenderConfig RenderContext::textConfig(domain::Font* font) const {
    return domain::TextRenderConfig{
        font,
        font_manager,
        default_face,
        ui_options.color_overrides
    };
}
