#ifndef GLYPHSEQUENCE_HPP_INCLUDED
#define GLYPHSEQUENCE_HPP_INCLUDED

#include "core/utf8string.hpp"
#include "opengl/font.hpp"
#include <memory>

class GlyphSequence
{
  public:
    GlyphSequence(std::shared_ptr<opengl::Font> font, const UTF8String &string);

    float width() const;

    float count() const;
    const std::vector<opengl::Glyph> &glyphs() const;

    void truncate(float max_width);

    UTF8String toUTF8String();

  private:
    std::vector<opengl::Glyph> m_glyphs;
    std::shared_ptr<opengl::Font> m_font;
};

#endif
