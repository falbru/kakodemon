#ifndef GLYPHSEQUENCE_HPP_INCLUDED
#define GLYPHSEQUENCE_HPP_INCLUDED

#include "domain/fontmanager.hpp"
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
    GlyphSequence(domain::Font *font, domain::FontManager *font_manager, const domain::UTF8String &string);
    GlyphSequence(domain::Font *font, domain::FontManager *font_manager,
                  const std::vector<domain::GlyphMetrics> &glyphs);

    float width() const;

    float count() const;
    const std::vector<domain::GlyphMetrics> &glyphs() const;

    void truncate(float max_width);
    std::optional<GlyphSequence> cut(float max_width, CutMode mode = CutMode::CHARACTER);
    GlyphSequence substr(int start, int length);

    domain::UTF8String toUTF8String();

  private:
    domain::Font *m_font;
    domain::FontManager *m_font_manager;
    std::vector<domain::GlyphMetrics> m_glyphs;
};

#endif
