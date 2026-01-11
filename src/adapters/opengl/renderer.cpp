#include "renderer.hpp"
#include "adapters/opengl/font.hpp"
#include "adapters/opengl/shaderprogram.hpp"
#include "domain/alignment.hpp"
#include "domain/color.hpp"
#include "domain/face.hpp"
#include "domain/glyphlinesbuilder.hpp"
#include "domain/line.hpp"
#include "domain/ports/fontengine.hpp"
#include "domain/utf8string.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "opengl.hpp"

opengl::Renderer::Renderer() {
}

void opengl::Renderer::init(int width, int height, domain::FontManager *font_manager) {
    m_shader_program = std::make_unique<ShaderProgram>();
    m_shader_program->compile();

    glGenVertexArrays(1, &m_text_vao);
    glGenBuffers(1, &m_text_vbo);
    glBindVertexArray(m_text_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_text_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &m_rect_vao);
    glGenBuffers(1, &m_rect_vbo);
    glBindVertexArray(m_rect_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_rect_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 2, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glBindVertexArray(0);

    onWindowResize(width, height);
}

void opengl::Renderer::onWindowResize(int width, int height) {
    glViewport(0, 0, width, height);

    glm::mat4 projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f);
    m_shader_program->use();
    m_shader_program->setMatrix4("projection", projection);
    m_screen_width = width;
    m_screen_height = height;
}

void opengl::Renderer::addBounds(float x, float y, float width, float height) {
    glEnable(GL_SCISSOR_TEST);
    domain::Rectangle r{x, m_screen_height - height - y, width, height};
    glScissor(r.x, r.y, r.width, r.height);
    m_bounds.push(r);
}

void opengl::Renderer::popBounds() {
    m_bounds.pop();
    if (m_bounds.empty()) {
        glDisable(GL_SCISSOR_TEST);
    }else {
        auto r = m_bounds.top();
        glScissor(r.x, r.y, r.width, r.height);
    }
}

void opengl::Renderer::renderLine(domain::Font* font, domain::FontManager* font_manager, const domain::Line &line, const domain::Face &default_face, float x,
                     float y) const {
    renderLine(font, font_manager, line, default_face, x, y, domain::Alignment());
}

void opengl::Renderer::renderLine(domain::Font* font, domain::FontManager* font_manager, const domain::Line &line, const domain::Face &default_face, float x, float y, const domain::Alignment& alignment) const
{
    opengl::Font* opengl_font = dynamic_cast<opengl::Font*>(font);

    if (!opengl_font) return;

    m_shader_program->use();
    glBindVertexArray(m_text_vao);

    _renderLine(opengl_font, font_manager, line, default_face, x, y, alignment, RenderPass::Both);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void opengl::Renderer::renderRect(const domain::RGBAColor color, float x, float y, float width, float height) const
{
    m_shader_program->use();

    _renderRect(color, x, y, width, height);

    glBindVertexArray(0);
}

void opengl::Renderer::renderRectWithShadow(const domain::RGBAColor color, float x, float y, float width, float height, float shadowRadius) const
{
    m_shader_program->use();

    _renderShadow(color, x, y, width, height, shadowRadius);
    _renderRect(color, x, y, width, height);

    glBindVertexArray(0);
}

void opengl::Renderer::renderRoundedRect(const domain::RGBAColor color, float x, float y, float width, float height, float corner_radius) const
{
    renderRoundedRect(color, x, y, width, height, corner_radius, corner_radius, corner_radius, corner_radius);
}

void opengl::Renderer::renderRoundedRect(const domain::RGBAColor color, float x, float y, float width, float height, float top_left_radius, float top_right_radius, float bottom_right_radius, float bottom_left_radius) const
{
    m_shader_program->use();

    _renderRoundedRect(color, x, y, width, height, top_left_radius, top_right_radius, bottom_right_radius, bottom_left_radius);

    glBindVertexArray(0);
}

void opengl::Renderer::renderRoundedRectWithShadow(const domain::RGBAColor color, float x, float y, float width, float height, float corner_radius, float shadowRadius) const
{
    renderRoundedRectWithShadow(color, x, y, width, height, corner_radius, corner_radius, corner_radius, corner_radius, shadowRadius);
}

void opengl::Renderer::renderRoundedRectWithShadow(const domain::RGBAColor color, float x, float y, float width, float height, float top_left_radius, float top_right_radius, float bottom_right_radius, float bottom_left_radius, float shadowRadius) const
{
    m_shader_program->use();

    _renderRoundedRectWithShadow(color, x, y, width, height, top_left_radius, top_right_radius, bottom_right_radius, bottom_left_radius, shadowRadius);
    _renderRoundedRect(color, x, y, width, height, top_left_radius, top_right_radius, bottom_right_radius, bottom_left_radius);

    glBindVertexArray(0);
}

void opengl::Renderer::renderLines(domain::Font* font, domain::FontManager* font_manager, const domain::Lines& lines, const domain::Face& default_face, float x, float y) const {
    opengl::Font* opengl_font = dynamic_cast<opengl::Font*>(font);

    if (!opengl_font) return;

    m_shader_program->use();

    float y_it = y;
    for (const auto& line : lines.getLines())
    {
        _renderLine(opengl_font, font_manager, line, default_face, x, y_it, domain::Alignment(), RenderPass::BackgroundOnly);
        y_it += font->getLineHeight();
    }

    glBindVertexArray(m_text_vao);
    y_it = y;
    for (const auto& line : lines.getLines())
    {
        _renderLine(opengl_font, font_manager, line, default_face, x, y_it, domain::Alignment(), RenderPass::TextOnly);
        y_it += font->getLineHeight();
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void opengl::Renderer::_renderLine(opengl::Font* font, domain::FontManager* font_manager, const domain::Line& line, const domain::Face& default_face, float x, float y, const domain::Alignment& alignment, RenderPass pass) const {

    float start_x = x;
    float start_y = y + font->getLineHeight();

    auto glyph_line = domain::GlyphLinesBuilder::build(line, font, font_manager);

    if (alignment.h == domain::Alignment::HorizontalAlignment::Right) {
        start_x -= glyph_line.width();
    } else if (alignment.h == domain::Alignment::HorizontalAlignment::Center) {
        start_x -= glyph_line.width() / 2.0f;
    }

    if (alignment.v == domain::Alignment::VerticalAlignment::Bottom) {
        start_y -= font->getLineHeight();
    } else if (alignment.v == domain::Alignment::VerticalAlignment::Center) {
        start_y -= font->getLineHeight() / 2.0f;
    }

    // First pass: Render all backgrounds
    if (pass == RenderPass::BackgroundOnly || pass == RenderPass::Both) {
        float x_it = start_x;
        float y_it = start_y;
        for (const auto& atom : glyph_line.getGlyphAtoms())
        {
            float height = font->getLineHeight();
            float width = atom.width();

            // Background color
            _renderRect(atom.getFace().getBg(default_face), x_it, y_it - height, width, height);

            x_it += width;
        }
    }

    // Second pass: Render all text
    if (pass == RenderPass::TextOnly || pass == RenderPass::Both) {
        float x_it = start_x;
        float y_it = start_y + font->getDescender();
        for (const auto& atom : glyph_line.getGlyphAtoms())
        {
            domain::RGBAColor color = atom.getFace().getFg(default_face);
            m_shader_program->setVector4f("textColor", color.r, color.g, color.b, color.a);

            float atom_x = x_it;

            for (const auto& run : atom.getRuns())
            {
                opengl::Font* run_font = dynamic_cast<opengl::Font*>(run.font);
                for (const auto& glyph : run.glyphs)
                {
                    if (domain::isControlCharacter(glyph.codepoint)) continue; // TODO clean way of stripping the last newline?

                    if (run_font->getGlyph(glyph.codepoint).format == domain::PixelFormat::GRAYSCALE) {
                        m_shader_program->setRenderType(RenderType::Text);
                    }else {
                        m_shader_program->setRenderType(RenderType::ColoredText);
                    }

                    float xpos = x_it + glyph.bearing.x;
                    float ypos = y_it - glyph.bearing.y;

                    float w = glyph.size.x;
                    float h = glyph.size.y;
                    float vertices[6][4] = {
                        {xpos, ypos, 0.0f, 0.0f}, {xpos, ypos + h, 0.0f, 1.0f},     {xpos + w, ypos + h, 1.0f, 1.0f},

                        {xpos, ypos, 0.0f, 0.0f}, {xpos + w, ypos + h, 1.0f, 1.0f}, {xpos + w, ypos, 1.0f, 0.0f}};

                    glBindVertexArray(m_text_vao);
                    glBindTexture(GL_TEXTURE_2D, run_font->getGlyph(glyph.codepoint).texture_id);
                    glBindBuffer(GL_ARRAY_BUFFER, m_text_vbo);
                    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
                    glBindBuffer(GL_ARRAY_BUFFER, 0);
                    glDrawArrays(GL_TRIANGLES, 0, 6);
                    x_it += glyph.advance;
                }
            }

            if (atom.getFace().hasAttribute(domain::Attribute::Underline) && font->getUnderlineThickness() > 0) {
                _renderRect(atom.getFace().getFg(default_face), atom_x, y_it + font->getUnderlineOffset(), x_it - atom_x, font->getUnderlineThickness());
            }
        }
    }
}

void opengl::Renderer::_renderShadow(const domain::RGBAColor color, float x, float y, float width, float height, float shadowRadius) const {
    float vertices[6][2] = {
        {x - shadowRadius, y - shadowRadius}, {x - shadowRadius, y + height + shadowRadius},     {x + width + shadowRadius, y + height + shadowRadius},
        {x - shadowRadius, y - shadowRadius}, {x + width + shadowRadius, y + height + shadowRadius}, {x + width + shadowRadius, y - shadowRadius}};

    glBindVertexArray(m_rect_vao);
    m_shader_program->setRenderType(RenderType::Shadow);
    m_shader_program->setFloat("shadowRadius", shadowRadius);
    m_shader_program->setVector4f("rectBounds", x, (float)m_screen_height - y - height, width, height);
    glBindBuffer(GL_ARRAY_BUFFER, m_rect_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void opengl::Renderer::_renderRect(const domain::RGBAColor color, float x, float y, float width, float height) const {
    float vertices[6][2] = {
        {x, y}, {x, y + height},     {x + width, y + height},
        {x, y}, {x + width, y + height}, {x + width, y}};

    glBindVertexArray(m_rect_vao);
    m_shader_program->setVector4f("rectColor", color.r, color.g, color.b, 1.0f);
    m_shader_program->setRenderType(RenderType::Rectangle);
    glBindBuffer(GL_ARRAY_BUFFER, m_rect_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void opengl::Renderer::_renderRoundedRect(const domain::RGBAColor color, float x, float y, float width, float height, float top_left_radius, float top_right_radius, float bottom_right_radius, float bottom_left_radius) const {
    float vertices[6][2] = {
        {x, y}, {x, y + height},     {x + width, y + height},
        {x, y}, {x + width, y + height}, {x + width, y}};

    glBindVertexArray(m_rect_vao);
    m_shader_program->setVector4f("rectColor", color.r, color.g, color.b, 1.0f);
    m_shader_program->setRenderType(RenderType::RoundedRectangle);
    m_shader_program->setVector4f("cornerRadii", bottom_left_radius, bottom_right_radius, top_right_radius, top_left_radius);
    m_shader_program->setVector4f("rectBounds", x, (float)m_screen_height - y - height, width, height);
    glBindBuffer(GL_ARRAY_BUFFER, m_rect_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void opengl::Renderer::_renderRoundedRectWithShadow(const domain::RGBAColor color, float x, float y, float width, float height, float top_left_radius, float top_right_radius, float bottom_right_radius, float bottom_left_radius, float shadowRadius) const {
    float vertices[6][2] = {
        {x - shadowRadius, y - shadowRadius}, {x - shadowRadius, y + height + shadowRadius},     {x + width + shadowRadius, y + height + shadowRadius},
        {x - shadowRadius, y - shadowRadius}, {x + width + shadowRadius, y + height + shadowRadius}, {x + width + shadowRadius, y - shadowRadius}};

    glBindVertexArray(m_rect_vao);
    m_shader_program->setRenderType(RenderType::RoundedShadow);
    m_shader_program->setFloat("shadowRadius", shadowRadius);
    m_shader_program->setVector4f("cornerRadii", bottom_left_radius, bottom_right_radius, top_right_radius, top_left_radius);
    m_shader_program->setVector4f("rectBounds", x, (float)m_screen_height - y - height, width, height);
    glBindBuffer(GL_ARRAY_BUFFER, m_rect_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
