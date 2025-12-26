#ifndef KAKOUNECONTENTVIEW_HPP_INCLUDED
#define KAKOUNECONTENTVIEW_HPP_INCLUDED

#include "domain/coord.hpp"
#include "domain/ports/renderer.hpp"

class KakouneContentView
{
  public:
    KakouneContentView();

    void init(domain::Renderer *renderer);

    void render(domain::Font *font, domain::FontManager *font_manager, const domain::Lines &lines,
                const domain::Face &default_face, float x, float y);

    float getCellWidth(domain::Font *font) const;
    float getCellHeight(domain::Font *font) const;

    void setWidth(float width);
    void setHeight(float height);
    float width() const;
    float height() const;

    std::pair<float, float> coordToPixels(domain::Font *font, const domain::Coord &coord) const;

  private:
    float m_width, m_height;
    domain::Renderer *m_renderer;
};

#endif
