#ifndef OPENGL_FONT_HPP_INCLUDED
#define OPENGL_FONT_HPP_INCLUDED

#include "opengl.hpp"
#include <freetype/freetype.h>
#include <ft2build.h>
#include <glm/vec2.hpp>
#include <map>
#include <string>

namespace opengl
{
struct Character
{
    unsigned int TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    long Advance;
};

class Font
{
  public:
    Font(const std::string &font_path, int font_size);
    ~Font();

    bool hasCharacter(char c) const;
    const Character &getCharacter(char c) const;

    void loadCharacter(char c);

    float getAscender() const;
    float getLineHeight() const;

  private:
    FT_Library m_ft;
    FT_Face m_face;

    float m_ascender;
    float m_line_height;
    std::map<char, Character> m_characters;
};
} // namespace opengl

#endif
