#ifndef GLYPHSEQUENCE_HPP_INCLUDED
#define GLYPHSEQUENCE_HPP_INCLUDED

#include "core/utf8string.hpp"
#include "opengl/font.hpp"
#include <memory>
#include <optional>

enum class CutMode
{
    CHARACTER,
    WORD
};

class GlyphSequence
{
  public:
    GlyphSequence(std::shared_ptr<opengl::Font> font, const UTF8String &string);
    GlyphSequence(std::shared_ptr<opengl::Font> font, const std::vector<opengl::Glyph> &glyphs);

    float width() const;

    float count() const;
    const std::vector<opengl::Glyph> &glyphs() const;

    void truncate(float max_width);
    std::optional<GlyphSequence> cut(float max_width, CutMode mode = CutMode::CHARACTER);
    GlyphSequence substr(int start, int length);

    UTF8String toUTF8String();

  private:
    std::vector<opengl::Glyph> m_glyphs;
    std::shared_ptr<opengl::Font> m_font;
};

#endif
