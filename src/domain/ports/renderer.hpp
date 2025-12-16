#ifndef DOMAIN_RENDERER_HPP_INCLUDED
#define DOMAIN_RENDERER_HPP_INCLUDED

#include "domain/alignment.hpp"
#include "domain/color.hpp"
#include "domain/line.hpp"
#include "domain/lines.hpp"
#include "domain/ports/font.hpp"

namespace domain
{

class Renderer
{
  public:
    virtual void init(int width, int height) = 0;
    virtual void onWindowResize(int width, int height) = 0;
    virtual void addBounds(float x, float y, float width, float height) = 0;
    virtual void popBounds() = 0;

    virtual void renderRect(const RGBAColor color, float x, float y, float width, float height) const = 0;
    virtual void renderRectWithShadow(const RGBAColor color, float x, float y, float width, float height,
                                      float shadowRadius) const = 0;
    virtual void renderLine(Font *font, const Line &line, const Face &default_face, float x, float y) const = 0;
    virtual void renderLine(Font *font, const Line &line, const Face &default_face, float x, float y,
                            const Alignment &alignment) const = 0;
    virtual void renderLines(Font *font, const Lines &lines, const domain::Face &default_face, float x,
                             float y) const = 0;
};

} // namespace domain

#endif // renderer_hpp_INCLUDED
