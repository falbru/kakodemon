#ifndef OPENGL_RENDERER_HPP_INCLUDED
#define OPENGL_RENDERER_HPP_INCLUDED

#include <memory>
#include <vector>

#include "core/alignment.hpp"
#include "core/color.hpp"
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

    void renderRect(const core::Color color, float x, float y, float width, float height) const;
    void renderRectWithShadow(const core::Color color, float x, float y, float width, float height,
                              float shadowRadius) const;
    void renderLine(std::shared_ptr<Font> font, const kakoune::Line &line, const kakoune::Face &default_face, float x,
                    float y) const;
    void renderLine(std::shared_ptr<Font> font, const kakoune::Line &line, const kakoune::Face &default_face, float x,
                    float y, const core::Alignment &alignment) const;
    void renderLines(std::shared_ptr<Font> font, const std::vector<kakoune::Line> &lines,
                     const kakoune::Face &default_face, float x, float y) const;

  private:
    void _renderLine(std::shared_ptr<Font> font, const kakoune::Line &line, const kakoune::Face &default_face, float x,
                     float y, const core::Alignment &alignment) const;
    void _renderShadow(const core::Color color, float x, float y, float width, float height, float shadowRadius) const;
    void _renderRect(const core::Color color, float x, float y, float width, float height) const;
    std::unique_ptr<ShaderProgram> m_shader_program;

    unsigned int m_text_vao, m_text_vbo;
    unsigned int m_rect_vao, m_rect_vbo;

    int m_screen_width, m_screen_height;
};

} // namespace opengl

#endif
