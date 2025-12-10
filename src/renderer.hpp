#ifndef RENDERER_HPP_INCLUDED
#define RENDERER_HPP_INCLUDED

#include "core/alignment.hpp"
#include "core/color.hpp"
#include "font.hpp"
#include "kakoune/line.hpp"

class Renderer
{
  public:
    virtual void init(int width, int height) = 0;
    virtual void onWindowResize(int width, int height) = 0;
    virtual void addBounds(float x, float y, float width, float height) = 0;
    virtual void popBounds() = 0;

    virtual void renderRect(const core::Color color, float x, float y, float width, float height) const = 0;
    virtual void renderRectWithShadow(const core::Color color, float x, float y, float width, float height,
                                      float shadowRadius) const = 0;
    virtual void renderLine(Font *font, const kakoune::Line &line, const kakoune::Face &default_face, float x,
                            float y) const = 0;
    virtual void renderLine(Font *font, const kakoune::Line &line, const kakoune::Face &default_face, float x, float y,
                            const core::Alignment &alignment) const = 0;
    virtual void renderLines(Font *font, const std::vector<kakoune::Line> &lines, const kakoune::Face &default_face,
                             float x, float y) const = 0;
};

#endif // renderer_hpp_INCLUDED
