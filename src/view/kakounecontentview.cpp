#include "kakounecontentview.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "kakounecontentview.hpp"
#include "spdlog/spdlog.h"
#include <freetype/freetype.h>
#include <glm/mat4x4.hpp>

const char *vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoords;
uniform int renderMode; // 0 = rectangle, 1 = text
out vec2 TexCoords;
uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(position, 0.0, 1.0);
    TexCoords = texCoords;
}
)";

const char *fragmentShaderSource = R"(
#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec4 textColor;
uniform vec4 rectColor;
uniform int renderMode; // 0 = rectangle, 1 = text

void main()
{
   if (renderMode == 0) {
       // Rectangle rendering
       color = rectColor;
   } else {
       // Text rendering
       vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
       color = textColor * sampled;
   }
}
)";

KakouneContentView::KakouneContentView() : m_shader_program(0), m_vao(0), m_vbo(0)
{
}

void KakouneContentView::init(int width, int height)
{
    FT_Face face;
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) // only init once
    {
        spdlog::error("ERROR::FREETYPE: Could not init FreeType Library");
    }

    if (FT_New_Face(ft, "/home/falk/.fonts/MonoLisa/ttf/MonoLisa-Regular.ttf", 0, &face))
    {
        spdlog::error("ERROR::FREETYPE: Failed to load font");
    }
    FT_Set_Pixel_Sizes(face, 0, 14);

    // load 128 chars
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            spdlog::error("ERROR::FREETYTPE: Failed to load Glyph");
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED,
                     GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {texture, glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                               glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top), face->glyph->advance.x};
        Characters.insert(std::pair<char, Character>(c, character));
    }

    m_ascender = face->size->metrics.ascender >> 6;
    m_line_height = face->size->metrics.height >> 6;

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        spdlog::error("ERROR::SHADER::VERTEX::COMPILATION_FAILED: {}", infoLog);
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        spdlog::error("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED:", infoLog);
    }
    // link shaders
    m_shader_program = glCreateProgram();
    glAttachShader(m_shader_program, vertexShader);
    glAttachShader(m_shader_program, fragmentShader);
    glLinkProgram(m_shader_program);
    // check for linking errors
    glGetProgramiv(m_shader_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_shader_program, 512, NULL, infoLog);
        spdlog::error("ERROR::SHADER::PROGRAM::LINKING_FAILED", infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    onWindowResize(width, height);

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // After your existing VAO setup, add:
    glGenVertexArrays(1, &m_rect_vao);
    glGenBuffers(1, &m_rect_vbo);
    glBindVertexArray(m_rect_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_rect_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 2, NULL, GL_DYNAMIC_DRAW); // 6 vertices, 2 floats each
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    // Don't enable attribute 1 for rectangles
    glBindVertexArray(0);
    // set up vertex data (an
}

void KakouneContentView::renderLine(const kakoune::Line &line, const kakoune::Face& default_face, float x, float y)
{
    float scale = 1.0f;

    for (auto atom : line.atoms)
    {
        auto color = atom.face.fg.toCoreColor(default_face.fg, true);
        glUniform4f(glGetUniformLocation(m_shader_program, "textColor"), color.r, color.g, color.b, color.a);
        std::string text = atom.contents;

        std::string::const_iterator c;
        float start_x = x;

        glUniform1i(glGetUniformLocation(m_shader_program, "renderMode"), 1);
        for (c = text.begin(); c != text.end(); c++)
        {
            Character ch = Characters[*c];
            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
        }

        // render background
        float h = m_line_height;
        float vertices[6][2] = {
            {start_x, y}, {start_x, y + h},     {x, y + h},
            {start_x, y}, {x, y + h}, {x, y}};
        // float vertices[6][2] = {
        //     {0.0f, 0.0f}, {0.0f, 0.0f + 20.0f},     {20.0f, 0.0f + 20.0f},
        //     {0.0f, 0.0f}, {20.0f, 0.0f + 20.0f}, {20.0f, 0.0f}};

        glBindVertexArray(m_rect_vao);
        color = atom.face.bg.toCoreColor(default_face.bg, false);
        glUniform4f(glGetUniformLocation(m_shader_program, "rectColor"), color.r, color.g, color.b, 1.0f);
        glUniform1i(glGetUniformLocation(m_shader_program, "renderMode"), 0);
        glBindBuffer(GL_ARRAY_BUFFER, m_rect_vbo);  // Use m_rect_vbo instead of m_vbo
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glUniform1i(glGetUniformLocation(m_shader_program, "renderMode"), 1);
        x = start_x;
        for (c = text.begin(); c != text.end(); c++)
        {
            if (*c == '\n') continue; // TODO clean way of stripping the last newline?

            Character ch = Characters[*c];

            float xpos = x + ch.Bearing.x;
            float ypos = y + m_ascender - ch.Bearing.y;

            float w = ch.Size.x * scale;
            float h = ch.Size.y * scale;
            // update VBO for each character
            float vertices[6][4] = {
                {xpos, ypos, 0.0f, 0.0f}, {xpos, ypos + h, 0.0f, 1.0f},     {xpos + w, ypos + h, 1.0f, 1.0f},

                {xpos, ypos, 0.0f, 0.0f}, {xpos + w, ypos + h, 1.0f, 1.0f}, {xpos + w, ypos, 1.0f, 0.0f}};
            glBindVertexArray(m_vao);
            // render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            // update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
        }
    }
}

void KakouneContentView::render(const std::vector<kakoune::Line> &lines, const kakoune::Face& default_face, float x, float y)
{
    glm::vec3 color = glm::vec3(1.0f);

    glUseProgram(m_shader_program);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(m_vao);

    float y_it = y;
    for (auto line : lines)
    {
        renderLine(line, default_face, x, y_it);
        y_it += m_line_height;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void KakouneContentView::onWindowResize(int width, int height)
{
    glm::mat4 projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f);
    glUseProgram(m_shader_program);
    glUniformMatrix4fv(glGetUniformLocation(m_shader_program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

int KakouneContentView::getCharWidth() {
    return Characters['A'].Advance >> 6;
}

int KakouneContentView::getCharHeight() {
    return m_line_height;
}
