#ifndef OPENGL_RENDERER_HPP_INCLUDED
#define OPENGL_RENDERER_HPP_INCLUDED

#include <memory>
#include <vector>

#include "kakoune/face.hpp"
#include "kakoune/line.hpp"
#include "opengl/font.hpp"
#include "opengl/shaderprogram.hpp"

namespace opengl
{

class Renderer
{
  public:
    Renderer();

    void init(int width, int height);
    void onWindowResize(int width, int height);

    void renderLine(Font &font, const kakoune::Line &line, const kakoune::Face &default_face, float x, float y) const;
    void renderLines(Font &font, const std::vector<kakoune::Line> &lines, const kakoune::Face &default_face, float x,
                     float y) const;

  private:
    std::unique_ptr<ShaderProgram> m_shader_program;

    unsigned int m_text_vao, m_text_vbo;
    unsigned int m_rect_vao, m_rect_vbo;
};

} // namespace opengl

#endif
