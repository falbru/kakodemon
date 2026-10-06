#include "renderer.hpp"
#include "adapters/opengl/font.hpp"
#include "adapters/opengl/shaderprogram.hpp"
#include "domain/alignment.hpp"
#include "domain/codepointstring.hpp"
#include "domain/color.hpp"
#include "domain/face.hpp"
#include "domain/ports/font.hpp"
#include "domain/ports/fontengine.hpp"
#include "domain/ports/renderer.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "opengl.hpp"

domain::FontFactory opengl::Renderer::getFontFactory()
{
    return [](domain::FontEngine *engine) -> std::unique_ptr<domain::Font> {
        return std::make_unique<opengl::Font>(engine);
    };
}

opengl::Renderer::Renderer()
{
}

opengl::Renderer::~Renderer()
{
}

void opengl::Renderer::init(int width, int height)
{
    m_shader_program = std::make_unique<ShaderProgram>();
    m_shader_program->compile();

    glGenVertexArrays(1, &m_text_vao);
    glGenBuffers(1, &m_text_vbo);
    glBindVertexArray(m_text_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_text_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &m_rect_vao);
    glGenBuffers(1, &m_rect_vbo);
    glBindVertexArray(m_rect_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_rect_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 2, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glBindVertexArray(0);

    onWindowResize(width, height);
}

void opengl::Renderer::onWindowResize(int width, int height)
{
    glViewport(0, 0, width, height);

    glm::mat4 projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f);
    m_shader_program->use();
    m_shader_program->setMatrix4("projection", projection);
    m_screen_width = width;
    m_screen_height = height;
}

void opengl::Renderer::addBounds(int x, int y, int width, int height)
{
    glEnable(GL_SCISSOR_TEST);
    domain::Rectangle r{x, static_cast<int>(m_screen_height) - height - y, width, height};
    glScissor(r.left(), r.top(), r.width(), r.height());
    m_bounds.push(r);
}

void opengl::Renderer::popBounds()
{
    m_bounds.pop();
    if (m_bounds.empty())
    {
        glDisable(GL_SCISSOR_TEST);
    }
    else
    {
        auto r = m_bounds.top();
        glScissor(r.left(), r.top(), r.width(), r.height());
    }
}

void opengl::Renderer::renderLine(const domain::TextRenderConfig &config, const domain::RenderLine &line, float x,
                                  float y, const domain::Alignment &alignment) const
{
    m_shader_program->use();
    glBindVertexArray(m_text_vao);

    float line_height = std::floor(config.font->getLineHeight());
    _renderLine(config, line, x, y, line_height, alignment, RenderPass::Both);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void opengl::Renderer::renderLines(const domain::TextRenderConfig &config, const domain::RenderLines &lines, float x,
                                   float y) const
{
    opengl::Font *opengl_font = dynamic_cast<opengl::Font *>(config.font);

    if (!opengl_font)
        return;

    const float line_height = std::floor(lines.getLineHeight());

    m_shader_program->use();

    float y_it = y;
    for (const auto &line : lines.getLines())
    {
        _renderLine(config, line, x, y_it, line_height, domain::Alignment(), RenderPass::BackgroundOnly);
        y_it += line_height;
    }

    glBindVertexArray(m_text_vao);
    y_it = y;
    for (const auto &line : lines.getLines())
    {
        _renderLine(config, line, x, y_it, line_height, domain::Alignment(), RenderPass::TextOnly);
        y_it += line_height;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void opengl::Renderer::renderRect(const domain::RGBAColor color, float x, float y, float width, float height) const
{
    m_shader_program->use();

    _renderRect(color, x, y, width, height);

    glBindVertexArray(0);
}

void opengl::Renderer::renderRectWithShadow(const domain::RGBAColor color, float x, float y, float width, float height,
                                            float shadowRadius) const
{
    m_shader_program->use();

    _renderShadow(color, x, y, width, height, shadowRadius);
    _renderRect(color, x, y, width, height);

    glBindVertexArray(0);
}

void opengl::Renderer::renderRoundedRect(const domain::RGBAColor color, float x, float y, float width, float height,
                                         domain::CornerRadius corner_radius) const
{
    m_shader_program->use();

    _renderRoundedRect(color, x, y, width, height, corner_radius);

    glBindVertexArray(0);
}

void opengl::Renderer::renderRoundedRectWithShadow(const domain::RGBAColor color, float x, float y, float width,
                                                   float height, domain::CornerRadius corner_radius,
                                                   float shadow_radius) const
{
    m_shader_program->use();

    _renderRoundedRectWithShadow(color, x, y, width, height, corner_radius, shadow_radius);
    _renderRoundedRect(color, x, y, width, height, corner_radius);

    glBindVertexArray(0);
}

void opengl::Renderer::_renderLine(const domain::TextRenderConfig &config, const domain::RenderLine &line, float x,
                                   float y, float line_height, const domain::Alignment &alignment,
                                   RenderPass pass) const
{
    opengl::Font *font = dynamic_cast<opengl::Font *>(config.font);

    float start_x = x;
    float start_y = y + line_height;

    if (alignment.h == domain::Alignment::HorizontalAlignment::Right)
    {
        start_x -= line.width();
    }
    else if (alignment.h == domain::Alignment::HorizontalAlignment::Center)
    {
        start_x -= line.width() / 2.0f;
    }

    if (alignment.v == domain::Alignment::VerticalAlignment::Bottom)
    {
        start_y -= line_height;
    }
    else if (alignment.v == domain::Alignment::VerticalAlignment::Center)
    {
        start_y -= line_height / 2.0f;
    }

    float font_line_height = font->getLineHeight();
    float vertical_offset = std::floor((line_height - font_line_height) / 2.0f);

    float x_it = start_x;
    float y_it = start_y + font->getDescender() - vertical_offset;

    const std::vector<domain::GlyphMetrics> &glyphs = line.getGlyphs();
    const std::vector<domain::Span<domain::ResolvedFace>> &face_spans = line.getFaceSpans();
    const std::vector<domain::Span<domain::Font *>> &font_spans = line.getFontSpans();

    if (pass == RenderPass::BackgroundOnly || pass == RenderPass::Both)
    {
        float x_it = start_x;
        float y_it = start_y;

        int glyph_index = 0;
        int face_span_index = 0;

        float atom_start_x = start_x;
        for (; glyph_index < glyphs.size(); glyph_index++)
        {
            if (face_span_index < face_spans.size() - 1 && glyph_index == face_spans[face_span_index + 1].start_index)
            {
                const auto &face = face_spans[face_span_index].value;

                _renderRect(face.getBg(), atom_start_x, y_it - line_height, x_it - atom_start_x, line_height);

                face_span_index++;
                atom_start_x = x_it;
            }

            const auto &glyph = glyphs[glyph_index];

            x_it += glyph.advance;
        }

        if (x_it - atom_start_x > 0 && face_span_index < face_spans.size())
        {
            const auto &face = face_spans[face_span_index].value;

            _renderRect(face.getBg(), atom_start_x, y_it - line_height, x_it - atom_start_x, line_height);
        }
    }

    if (pass == RenderPass::TextOnly || pass == RenderPass::Both)
    {
        int glyph_index = 0;
        int face_span_index = 0;
        int font_span_index = 0;

        float atom_start_x = 0;
        for (; glyph_index < glyphs.size(); glyph_index++)
        {
            if (face_span_index < face_spans.size() - 1 && glyph_index == face_spans[face_span_index + 1].start_index)
            {
                const auto &current_face = face_spans[face_span_index].value;

                if (current_face.hasAttribute(domain::Attribute::Underline) && font->getUnderlineThickness() > 0)
                {
                    _renderRect(current_face.getFg(), atom_start_x, y_it + font->getUnderlineOffset(),
                                x_it - atom_start_x, font->getUnderlineThickness());
                }

                face_span_index++;
                atom_start_x = x_it;
            }

            if (font_span_index < font_spans.size() - 1 && glyph_index == font_spans[font_span_index + 1].start_index)
            {
                font_span_index++;
            }

            const auto &glyph = glyphs[glyph_index];
            const auto &face = face_spans[face_span_index].value;
            const opengl::Font *glyph_font = dynamic_cast<opengl::Font *>(font_spans[font_span_index].value);

            if (domain::isControlCharacter(glyph.codepoint))
                continue;

            const opengl::Glyph &opengl_glyph = glyph_font->getGlyph(glyph.codepoint);

            if (opengl_glyph.format == domain::PixelFormat::GRAYSCALE)
            {
                m_shader_program->setRenderType(RenderType::Text);
            }
            else
            {
                m_shader_program->setRenderType(RenderType::ColoredText);
            }

            domain::RGBAColor color = face.getFg();
            m_shader_program->setVector4f("textColor", color.r, color.g, color.b, color.a);

            float xpos = x_it + glyph.bearing.x;
            float ypos = y_it - glyph.bearing.y;

            float w = glyph.size.x;
            float h = glyph.size.y;
            float vertices[6][4] = {
                {xpos, ypos, 0.0f, 0.0f}, {xpos, ypos + h, 0.0f, 1.0f},     {xpos + w, ypos + h, 1.0f, 1.0f},

                {xpos, ypos, 0.0f, 0.0f}, {xpos + w, ypos + h, 1.0f, 1.0f}, {xpos + w, ypos, 1.0f, 0.0f}};

            glBindVertexArray(m_text_vao);
            glBindTexture(GL_TEXTURE_2D, opengl_glyph.texture_id);
            glBindBuffer(GL_ARRAY_BUFFER, m_text_vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            x_it += glyph.advance;
        }
    }
}

void opengl::Renderer::_renderShadow(const domain::RGBAColor color, float x, float y, float width, float height,
                                     float shadowRadius) const
{
    float vertices[6][2] = {{x - shadowRadius, y - shadowRadius},
                            {x - shadowRadius, y + height + shadowRadius},
                            {x + width + shadowRadius, y + height + shadowRadius},
                            {x - shadowRadius, y - shadowRadius},
                            {x + width + shadowRadius, y + height + shadowRadius},
                            {x + width + shadowRadius, y - shadowRadius}};

    glBindVertexArray(m_rect_vao);
    m_shader_program->setRenderType(RenderType::Shadow);
    m_shader_program->setFloat("shadowRadius", shadowRadius);
    m_shader_program->setVector4f("rectBounds", x, (float)m_screen_height - y - height, width, height);
    glBindBuffer(GL_ARRAY_BUFFER, m_rect_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void opengl::Renderer::_renderRect(const domain::RGBAColor color, float x, float y, float width, float height) const
{
    float vertices[6][2] = {{x, y}, {x, y + height},         {x + width, y + height},
                            {x, y}, {x + width, y + height}, {x + width, y}};

    glBindVertexArray(m_rect_vao);
    m_shader_program->setVector4f("rectColor", color.r, color.g, color.b, color.a);
    m_shader_program->setRenderType(RenderType::Rectangle);
    glBindBuffer(GL_ARRAY_BUFFER, m_rect_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void opengl::Renderer::_renderRoundedRect(const domain::RGBAColor color, float x, float y, float width, float height,
                                          domain::CornerRadius corner_radius) const
{
    float vertices[6][2] = {{x, y}, {x, y + height},         {x + width, y + height},
                            {x, y}, {x + width, y + height}, {x + width, y}};

    glBindVertexArray(m_rect_vao);
    m_shader_program->setVector4f("rectColor", color.r, color.g, color.b, 1.0f);
    m_shader_program->setRenderType(RenderType::RoundedRectangle);
    m_shader_program->setVector4f("cornerRadii", corner_radius.bottom_left, corner_radius.bottom_right,
                                  corner_radius.top_right, corner_radius.top_left);
    m_shader_program->setVector4f("rectBounds", x, (float)m_screen_height - y - height, width, height);
    glBindBuffer(GL_ARRAY_BUFFER, m_rect_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void opengl::Renderer::_renderRoundedRectWithShadow(const domain::RGBAColor color, float x, float y, float width,
                                                    float height, domain::CornerRadius corner_radius,
                                                    float shadow_radius) const
{
    float vertices[6][2] = {{x - shadow_radius, y - shadow_radius},
                            {x - shadow_radius, y + height + shadow_radius},
                            {x + width + shadow_radius, y + height + shadow_radius},
                            {x - shadow_radius, y - shadow_radius},
                            {x + width + shadow_radius, y + height + shadow_radius},
                            {x + width + shadow_radius, y - shadow_radius}};

    glBindVertexArray(m_rect_vao);
    m_shader_program->setRenderType(RenderType::RoundedShadow);
    m_shader_program->setFloat("shadowRadius", shadow_radius);
    m_shader_program->setVector4f("cornerRadii", corner_radius.bottom_left, corner_radius.bottom_right,
                                  corner_radius.top_right, corner_radius.top_left);
    m_shader_program->setVector4f("rectBounds", x, (float)m_screen_height - y - height, width, height);
    glBindBuffer(GL_ARRAY_BUFFER, m_rect_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
