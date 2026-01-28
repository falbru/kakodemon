#ifndef KAKOUNECONTENTVIEW_HPP_INCLUDED
#define KAKOUNECONTENTVIEW_HPP_INCLUDED

#include "application/view/rendercontext.hpp"
#include "domain/coord.hpp"
#include "domain/geometry.hpp"
#include "domain/ports/renderer.hpp"

class KakouneContentView
{
  public:
    KakouneContentView();

    void init(domain::Renderer *renderer);

    void render(const RenderContext &render_context, const domain::Lines &lines, const domain::Face &default_face,
                const domain::Rectangle &bounds);

    float getCellWidth(domain::Font *font) const;
    float getCellHeight(domain::Font *font) const;

    std::pair<float, float> coordToPixels(domain::Font *font, const domain::Coord &coord, float origin_x,
                                          float origin_y) const;
    domain::Coord pixelToCoord(domain::Font *font, float x, float y, float origin_x, float origin_y) const;

  private:
    domain::Renderer *m_renderer;
};

#endif
