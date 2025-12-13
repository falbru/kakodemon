#ifndef GLYPHSEQUENCE_HPP_INCLUDED
#define GLYPHSEQUENCE_HPP_INCLUDED

#include "domain/ports/font.hpp"
#include "domain/utf8string.hpp"
#include <optional>

enum class CutMode
{
    CHARACTER,
    WORD
};

class GlyphSequence
{
  public:
    GlyphSequence(Font *font, const UTF8String &string);
    GlyphSequence(Font *font, const std::vector<GlyphMetrics> &glyphs);

    float width() const;

    float count() const;
    const std::vector<GlyphMetrics> &glyphs() const;

    void truncate(float max_width);
    std::optional<GlyphSequence> cut(float max_width, CutMode mode = CutMode::CHARACTER);
    GlyphSequence substr(int start, int length);

    UTF8String toUTF8String();

  private:
    std::vector<GlyphMetrics> m_glyphs;

    Font *m_font;
};

#endif
