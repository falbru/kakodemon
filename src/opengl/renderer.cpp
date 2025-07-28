#include "renderer.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "opengl/font.hpp"

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
    for (auto atom : line.atoms)
    {
        auto color = atom.face.fg.toCoreColor(default_face.fg, true);
        m_shader_program->setVector4f("textColor", color.r, color.g, color.b, color.a);
        std::string text = atom.contents;

        std::string::const_iterator c;
        float start_x = x;

        for (c = text.begin(); c != text.end(); c++)
        {
            auto ch = font.getCharacter(*c);
            x += ch.Advance >> 6;
        }

        // render background
        float h = font.getLineHeight();
        float vertices[6][2] = {
            {start_x, y}, {start_x, y + h},     {x, y + h},
            {start_x, y}, {x, y + h}, {x, y}};

        glBindVertexArray(m_rect_vao);
        color = atom.face.bg.toCoreColor(default_face.bg, false);
        m_shader_program->setVector4f("rectColor", color.r, color.g, color.b, 1.0f);
        m_shader_program->setInt("renderType", 1);
        glBindBuffer(GL_ARRAY_BUFFER, m_rect_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        m_shader_program->setInt("renderType", 0);
        x = start_x;
        for (c = text.begin(); c != text.end(); c++)
        {
            if (*c == '\n') continue; // TODO clean way of stripping the last newline?

            auto ch = font.getCharacter(*c);

            float xpos = x + ch.Bearing.x;
            float ypos = y + font.getAscender() - ch.Bearing.y;

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
            x += ch.Advance >> 6; // bitshift by 6 to get value in pixels (2^6 = 64)
        }
    }
}

void opengl::Renderer::renderLines(Font& font, const std::vector<kakoune::Line>& lines, const kakoune::Face& default_face, float x, float y) const {
    m_shader_program->use();
    glBindVertexArray(m_text_vao);

    float y_it = y;
    for (auto line : lines)
    {
        renderLine(font, line, default_face, x, y_it);
        y_it += font.getLineHeight();
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
