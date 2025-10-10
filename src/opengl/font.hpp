#ifndef OPENGL_FONT_HPP_INCLUDED
#define OPENGL_FONT_HPP_INCLUDED

#include "core/utf8string.hpp"
#include "opengl.hpp"
#include <X11/extensions/render.h>
#include <freetype/freetype.h>
#include <ft2build.h>
#include <glm/vec2.hpp>
#include <map>
#include <string>

namespace opengl
{
struct Glyph
{
    Codepoint codepoint;
    unsigned int TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    long Advance;

    float width() const;
};

class Font
{
  public:
    Font(const std::string &font_path, int font_size);
    ~Font();

    bool hasGlyph(Codepoint c) const;
    const Glyph &getGlyph(Codepoint c) const;

    void loadGlyph(Codepoint c);
    const Glyph &ensureGlyph(Codepoint c);

    float getAscender() const;
    float getLineHeight() const;

    float width(UTF8String string);

  private:
    FT_Library m_ft;
    FT_Face m_face;

    float m_ascender;
    float m_line_height;
    std::map<Codepoint, Glyph> m_glyphs;
};
} // namespace opengl

#endif
