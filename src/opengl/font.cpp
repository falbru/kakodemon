#include "font.hpp"

#include <spdlog/spdlog.h>

opengl::Font::Font(const std::string& font_path, int font_size) {
    if (FT_Init_FreeType(&m_ft))
    {
        spdlog::error("ERROR::FREETYPE: Could not init FreeType Library");
    }

    if (FT_New_Face(m_ft, font_path.c_str(), 0, &m_face))
    {
        spdlog::error("ERROR::FREETYPE: Failed to load font");
    }
    FT_Set_Pixel_Sizes(m_face, 0, font_size);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++)
    {
        loadCharacter(c);
    }

    m_ascender = m_face->size->metrics.ascender >> 6;
    m_line_height = m_face->size->metrics.height >> 6;
}

opengl::Font::~Font() {
    FT_Done_Face(m_face);
    FT_Done_FreeType(m_ft);
}

bool opengl::Font::hasCharacter(char c) const {
    return m_characters.find(c) != m_characters.end();
}

const opengl::Character& opengl::Font::getCharacter(char c) const {
    return m_characters.at(c);
}

void opengl::Font::loadCharacter(char c) {
    if (FT_Load_Char(m_face, c, FT_LOAD_RENDER))
    {
        spdlog::error("ERROR::FREETYTPE: Failed to load Glyph");
        return;
    }

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows, 0, GL_RED,
                 GL_UNSIGNED_BYTE, m_face->glyph->bitmap.buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    Character character = {texture, glm::ivec2(m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows),
                           glm::ivec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top), m_face->glyph->advance.x};
    m_characters.insert(std::pair<char, Character>(c, character));
}

float opengl::Font::getAscender() const {
    return m_ascender;
}

float opengl::Font::getLineHeight() const {
    return m_line_height;
}
