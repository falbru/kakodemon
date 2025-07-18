#ifndef KAKOUNECONTENTVIEW_HPP_INCLUDED
#define KAKOUNECONTENTVIEW_HPP_INCLUDED

#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "model/line.hpp"
#include "opengl.hpp"
#include <ft2build.h>
#include <vector>
#include FT_FREETYPE_H

struct Character
{
    unsigned int TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    long Advance;
};

class KakouneContentView
{
  public:
    KakouneContentView();

    void init(int width, int height);
    void render(const std::vector<Line> &lines, float x, float y);
    void onWindowResize(int width, int height);

  private:
    void renderLine(const Line &line, float x, float y);
    unsigned int m_shader_program;
    unsigned int m_vao, m_vbo;

    float m_ascender;
    float m_line_height;
    std::map<char, Character> Characters;
};

#endif
