#include "renderer.hpp"
#include "core/alignment.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "opengl/font.hpp"
#include "spdlog/spdlog.h"

opengl::Renderer::Renderer() {
}

void opengl::Renderer::init(int width, int height) {
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
    glm::mat4 projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f);
    m_shader_program->setMatrix4("projection", projection, true);
}

void opengl::Renderer::renderLine(Font& font, const kakoune::Line& line, const kakoune::Face& default_face, float x, float y) const {
    renderLine(font, line, default_face, x, y, core::Alignment());
}

void opengl::Renderer::renderLine(Font &font, const kakoune::Line &line, const kakoune::Face &default_face, float x, float y, const core::Alignment& alignment) const
{
    m_shader_program->use();
    glBindVertexArray(m_text_vao);

    _renderLine(font, line, default_face, x, y, alignment);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void opengl::Renderer::renderLines(Font& font, const std::vector<kakoune::Line>& lines, const kakoune::Face& default_face, float x, float y) const {
    m_shader_program->use();
    glBindVertexArray(m_text_vao);

    float y_it = y;
    for (auto line : lines)
    {
        _renderLine(font, line, default_face, x, y_it, core::Alignment());
        y_it += font.getLineHeight();
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void opengl::Renderer::_renderLine(Font& font, const kakoune::Line& line, const kakoune::Face& default_face, float x, float y, const core::Alignment& alignment) const {
    std::string::const_iterator c;

    float start_x = x;
    float start_y = y;

    if (alignment.h == core::Alignment::HorizontalAlignment::Right) {
        float width = 0;
        for (auto atom : line.atoms) {
            for (c = atom.contents.begin(); c != atom.contents.end(); c++) {
                auto ch = font.getCharacter(*c);
                width += ch.Advance >> 6;
            }
        }

        start_x -= width;
    }

    if (alignment.v == core::Alignment::VerticalAlignment::Bottom) {
        start_y -= font.getLineHeight();
    }

    float x_it = start_x;
    float y_it = start_y;
    for (auto atom : line.atoms)
    {
        std::string text = atom.contents;


        float height = font.getLineHeight();
        float width = 0;
        for (c = text.begin(); c != text.end(); c++)
        {
            if (!font.hasCharacter(*c)) {
                font.loadCharacter(*c);
            }
            auto ch = font.getCharacter(*c);
            width += ch.Advance >> 6;
        }

        // Background color
        _renderRect(atom.face.bg.toCoreColor(default_face.bg, false), x_it, y_it, width, height);

        auto color = atom.face.fg.toCoreColor(default_face.fg, true);
        m_shader_program->setVector4f("textColor", color.r, color.g, color.b, color.a);
        m_shader_program->setInt("renderType", 0);
        for (c = text.begin(); c != text.end(); c++)
        {
            if (*c == '\n') continue; // TODO clean way of stripping the last newline?

            if (!font.hasCharacter(*c)) {
                font.loadCharacter(*c);
            }
            auto ch = font.getCharacter(*c);

            float xpos = x_it + ch.Bearing.x;
            float ypos = y_it + font.getAscender() - ch.Bearing.y;

            float w = ch.Size.x;
            float h = ch.Size.y;
            // update VBO for each character
            float vertices[6][4] = {
                {xpos, ypos, 0.0f, 0.0f}, {xpos, ypos + h, 0.0f, 1.0f},     {xpos + w, ypos + h, 1.0f, 1.0f},

                {xpos, ypos, 0.0f, 0.0f}, {xpos + w, ypos + h, 1.0f, 1.0f}, {xpos + w, ypos, 1.0f, 0.0f}};
            glBindVertexArray(m_text_vao);
            // render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            // update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, m_text_vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x_it += ch.Advance >> 6; // bitshift by 6 to get value in pixels (2^6 = 64)
        }
    }
}

void opengl::Renderer::_renderRect(const core::Color color, float x, float y, float width, float height) const {
    float vertices[6][2] = {
        {x, y}, {x, y + height},     {x + width, y + height},
        {x, y}, {x + width, y + height}, {x + width, y}};

    glBindVertexArray(m_rect_vao);
    m_shader_program->setVector4f("rectColor", color.r, color.g, color.b, 1.0f);
    m_shader_program->setInt("renderType", 1);
    glBindBuffer(GL_ARRAY_BUFFER, m_rect_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
