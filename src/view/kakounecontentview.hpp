#ifndef KAKOUNECONTENTVIEW_HPP_INCLUDED
#define KAKOUNECONTENTVIEW_HPP_INCLUDED

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "kakoune/coord.hpp"
#include "kakoune/face.hpp"
#include "kakoune/line.hpp"
#include "opengl/font.hpp"
#include "opengl/opengl.hpp"
#include "opengl/renderer.hpp"
#include <ft2build.h>
#include <vector>
#include FT_FREETYPE_H

class KakouneContentView
{
  public:
    KakouneContentView();

    void init(std::shared_ptr<opengl::Renderer> renderer);

    void render(const std::vector<kakoune::Line> &lines, const kakoune::Face &default_face, float x, float y);

    float getCellWidth() const;
    float getCellHeight() const;

    void setWidth(float width);
    void setHeight(float height);
    float width() const;
    float height() const;

    std::pair<float, float> coordToPixels(const kakoune::Coord &coord) const;

  private:
    float m_width, m_height;
    std::shared_ptr<opengl::Renderer> m_renderer;
    std::shared_ptr<opengl::Font> m_font;
};

#endif
