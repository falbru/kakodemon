#ifndef OPENGL_RENDERER_HPP_INCLUDED
#define OPENGL_RENDERER_HPP_INCLUDED

#include <memory>
#include <stack>

#include "domain/alignment.hpp"
#include "domain/color.hpp"
#include "domain/ports/renderer.hpp"
#include "font.hpp"
#include "shaderprogram.hpp"

namespace opengl
{

enum class RenderPass
{
    BackgroundOnly,
    TextOnly,
    Both
};

class Renderer : public domain::Renderer
{
  public:
    Renderer();

    void init(int width, int height, domain::FontManager *font_manager) override;
    void onWindowResize(int width, int height) override;
    void addBounds(float x, float y, float width, float height) override;
    void popBounds() override;

    void renderRect(const domain::RGBAColor color, float x, float y, float width, float height) const override;
    void renderRectWithShadow(const domain::RGBAColor color, float x, float y, float width, float height,
                              float shadowRadius) const override;
    void renderRoundedRect(const domain::RGBAColor color, float x, float y, float width, float height,
                           float corner_radius) const override;
    void renderRoundedRect(const domain::RGBAColor color, float x, float y, float width, float height,
                           float top_left_radius, float top_right_radius, float bottom_right_radius,
                           float bottom_left_radius) const override;
    void renderLine(domain::Font *font, domain::FontManager *font_manager, const domain::Line &line,
                    const domain::Face &default_face, float x, float y) const override;
    void renderLine(domain::Font *font, domain::FontManager *font_manager, const domain::Line &line,
                    const domain::Face &default_face, float x, float y,
                    const domain::Alignment &alignment) const override;
    void renderLines(domain::Font *font, domain::FontManager *font_manager, const domain::Lines &lines,
                     const domain::Face &default_face, float x, float y) const override;

  private:
    void _renderLine(opengl::Font *font, domain::FontManager *font_manager, const domain::Line &line,
                     const domain::Face &default_face, float x, float y, const domain::Alignment &alignment,
                     RenderPass pass) const;
    void _renderShadow(const domain::RGBAColor color, float x, float y, float width, float height,
                       float shadowRadius) const;
    void _renderRect(const domain::RGBAColor color, float x, float y, float width, float height) const;
    void _renderRoundedRect(const domain::RGBAColor color, float x, float y, float width, float height,
                            float top_left_radius, float top_right_radius, float bottom_right_radius,
                            float bottom_left_radius) const;
    std::unique_ptr<ShaderProgram> m_shader_program;

    unsigned int m_text_vao, m_text_vbo;
    unsigned int m_rect_vao, m_rect_vbo;

    unsigned int m_screen_width, m_screen_height;

    std::stack<domain::Rectangle> m_bounds;
};

} // namespace opengl

#endif
