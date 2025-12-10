#ifndef OPENGL_RENDERER_HPP_INCLUDED
#define OPENGL_RENDERER_HPP_INCLUDED

#include <memory>
#include <stack>
#include <vector>

#include "../renderer.hpp"
#include "core/alignment.hpp"
#include "core/color.hpp"
#include "kakoune/face.hpp"
#include "kakoune/line.hpp"
#include "opengl/font.hpp"
#include "opengl/shaderprogram.hpp"

namespace opengl
{

class Renderer : public ::Renderer
{
  public:
    Renderer();

    void init(int width, int height) override;
    void onWindowResize(int width, int height) override;
    void addBounds(float x, float y, float width, float height) override;
    void popBounds() override;

    void renderRect(const core::Color color, float x, float y, float width, float height) const override;
    void renderRectWithShadow(const core::Color color, float x, float y, float width, float height,
                              float shadowRadius) const override;
    void renderLine(::Font *font, const kakoune::Line &line, const kakoune::Face &default_face, float x,
                    float y) const override;
    void renderLine(::Font *font, const kakoune::Line &line, const kakoune::Face &default_face, float x, float y,
                    const core::Alignment &alignment) const override;
    void renderLines(::Font *font, const std::vector<kakoune::Line> &lines, const kakoune::Face &default_face, float x,
                     float y) const override;

  private:
    void _renderLine(opengl::Font *font, const kakoune::Line &line, const kakoune::Face &default_face, float x, float y,
                     const core::Alignment &alignment) const;
    void _renderShadow(const core::Color color, float x, float y, float width, float height, float shadowRadius) const;
    void _renderRect(const core::Color color, float x, float y, float width, float height) const;
    std::unique_ptr<ShaderProgram> m_shader_program;

    unsigned int m_text_vao, m_text_vbo;
    unsigned int m_rect_vao, m_rect_vbo;

    unsigned int m_screen_width, m_screen_height;

    std::stack<Rectangle> m_bounds;
};

} // namespace opengl

#endif
