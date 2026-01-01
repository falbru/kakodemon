#ifndef OPENGL_FONT_HPP_INCLUDED
#define OPENGL_FONT_HPP_INCLUDED

#include "domain/ports/font.hpp"
#include "domain/ports/fontengine.hpp"
#include "domain/utf8string.hpp"
#include "opengl.hpp"
#include <map>

namespace opengl
{

struct Glyph
{
    unsigned int texture_id;
    domain::GlyphMetrics metrics;
    domain::PixelFormat format;
};

class Font : public domain::Font
{
  public:
    Font(domain::FontEngine *font_engine);
    virtual ~Font();

    bool hasGlyph(domain::Codepoint c) const override;
    const domain::GlyphMetrics &getGlyphMetrics(domain::Codepoint c) const override;
    const domain::GlyphMetrics &getFallbackGlyphMetrics() const override;

    const Glyph &getGlyph(domain::Codepoint c) const;

    bool loadGlyph(domain::Codepoint c) override;

    float getAscender() const override;
    float getLineHeight() const override;
    int getSize() const override;

  private:
    std::optional<Glyph> fallback_glyph;
    std::map<domain::Codepoint, Glyph> m_glyphs;
    domain::FontEngine *m_font_engine;
};

} // namespace opengl

#endif
