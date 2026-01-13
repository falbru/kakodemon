#ifndef VIEW_RENDERCONTEXT_HPP_INCLUDED
#define VIEW_RENDERCONTEXT_HPP_INCLUDED

#include "domain/face.hpp"
#include "domain/fontmanager.hpp"
#include "domain/ports/renderer.hpp"
#include "domain/uioptions.hpp"

struct RenderContext
{
    domain::FontManager *font_manager;
    domain::Face default_face;
    domain::UIOptions ui_options;

    float screen_width;
    float screen_height;

    domain::TextRenderConfig textConfig(domain::Font *font) const;
};

#endif
