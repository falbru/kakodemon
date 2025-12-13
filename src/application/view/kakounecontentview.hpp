#ifndef KAKOUNECONTENTVIEW_HPP_INCLUDED
#define KAKOUNECONTENTVIEW_HPP_INCLUDED

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "adapters/opengl/opengl.hpp"
#include "domain/ports/renderer.hpp"
#include "kakoune/coord.hpp"
#include "kakoune/face.hpp"
#include "kakoune/line.hpp"
#include <vector>

class KakouneContentView
{
  public:
    KakouneContentView();

    void init(domain::Renderer *renderer);

    void render(domain::Font *font, const std::vector<kakoune::Line> &lines, const kakoune::Face &default_face, float x,
                float y);

    float getCellWidth(domain::Font *font) const;
    float getCellHeight(domain::Font *font) const;

    void setWidth(float width);
    void setHeight(float height);
    float width() const;
    float height() const;

    std::pair<float, float> coordToPixels(domain::Font *font, const kakoune::Coord &coord) const;

  private:
    float m_width, m_height;
    domain::Renderer *m_renderer;
};

#endif
