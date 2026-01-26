#ifndef OPENGL_RENDERER_HPP_INCLUDED
#define OPENGL_RENDERER_HPP_INCLUDED

#include <memory>
#include <stack>

#include "domain/alignment.hpp"
#include "domain/color.hpp"
#include "domain/ports/renderer.hpp"
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
    ~Renderer() override;

    void init(int width, int height) override;
    void onWindowResize(int width, int height) override;
    void addBounds(float x, float y, float width, float height) override;
    void popBounds() override;

    void renderRect(const domain::RGBAColor color, float x, float y, float width, float height) const override;
    void renderRectWithShadow(const domain::RGBAColor color, float x, float y, float width, float height,
                              float shadow_radius) const override;
    void renderRoundedRect(const domain::RGBAColor color, float x, float y, float width, float height,
                           domain::CornerRadius corner_radius) const override;
    void renderRoundedRectWithShadow(const domain::RGBAColor color, float x, float y, float width, float height,
                                     domain::CornerRadius corner_radius, float shadow_radius) const override;
    void renderLine(const domain::TextRenderConfig &config, const domain::Line &line, const domain::Face &default_face,
                    float x, float y, const domain::Alignment &alignment = domain::Alignment()) const override;
    void renderLines(const domain::TextRenderConfig &config, const domain::Lines &lines,
                     const domain::Face &default_face, float x, float y) const override;

  private:
    void _renderLine(const domain::TextRenderConfig &config, const domain::Line &line, const domain::Face &default_face,
                     float x, float y, const domain::Alignment &alignment, RenderPass pass) const;
    void _renderShadow(const domain::RGBAColor color, float x, float y, float width, float height,
                       float shadowRadius) const;
    void _renderRect(const domain::RGBAColor color, float x, float y, float width, float height) const;
    void _renderRoundedRect(const domain::RGBAColor color, float x, float y, float width, float height,
                            domain::CornerRadius corner_radius) const;
    void _renderRoundedRectWithShadow(const domain::RGBAColor color, float x, float y, float width, float height,
                                      domain::CornerRadius corner_radius, float shadow_radius) const;
    std::unique_ptr<ShaderProgram> m_shader_program;

    unsigned int m_text_vao, m_text_vbo;
    unsigned int m_rect_vao, m_rect_vbo;

    unsigned int m_screen_width, m_screen_height;

    std::stack<domain::Rectangle> m_bounds;
};

} // namespace opengl

#endif
