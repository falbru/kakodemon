#ifndef DOMAIN_RENDERER_HPP_INCLUDED
#define DOMAIN_RENDERER_HPP_INCLUDED

#include "domain/alignment.hpp"
#include "domain/color.hpp"
#include "domain/line.hpp"
#include "domain/lines.hpp"
#include "domain/ports/font.hpp"
#include <unordered_map>

namespace domain
{

class FontManager;

struct TextRenderConfig
{
    domain::Font *font;
    domain::FontManager *font_manager;
    const domain::Face &default_face;
    const std::unordered_map<domain::FixedColor, domain::RGBAColor> &color_overrides;
};

struct CornerRadius
{
    float top_left;
    float top_right;
    float bottom_right;
    float bottom_left;

    CornerRadius(float radius = 0.0f);
    CornerRadius(float top_left, float top_right, float bottom_right, float bottom_left);
};

class Renderer
{
  public:
    virtual void init(int width, int height) = 0;
    virtual void onWindowResize(int width, int height) = 0;
    virtual void addBounds(float x, float y, float width, float height) = 0;
    virtual void popBounds() = 0;

    virtual void renderRect(const RGBAColor color, float x, float y, float width, float height) const = 0;
    virtual void renderRectWithShadow(const RGBAColor color, float x, float y, float width, float height,
                                      float shadow_radius) const = 0;
    virtual void renderRoundedRect(const RGBAColor color, float x, float y, float width, float height,
                                   CornerRadius corner_radius) const = 0;
    virtual void renderRoundedRectWithShadow(const RGBAColor color, float x, float y, float width, float height,
                                             CornerRadius corner_radius, float shadow_radius) const = 0;
    virtual void renderLine(const TextRenderConfig &config, const Line &line, const Face &default_face, float x,
                            float y, const Alignment &alignment = Alignment()) const = 0;
    virtual void renderLines(const TextRenderConfig &config, const Lines &lines, const domain::Face &default_face,
                             float x, float y) const = 0;
};

} // namespace domain

#endif // renderer_hpp_INCLUDED
