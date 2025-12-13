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
};

class Font : public domain::Font
{
  public:
    Font(domain::FontEngine *font_engine);
    virtual ~Font();

    bool hasGlyph(domain::Codepoint c) const override;
    const domain::GlyphMetrics &getGlyphMetrics(domain::Codepoint c) const override;

    const Glyph &getGlyph(domain::Codepoint c) const;

    void loadGlyph(domain::Codepoint c) override;
    const domain::GlyphMetrics &ensureGlyph(domain::Codepoint c) override;

    float getAscender() const override;
    float getLineHeight() const override;

    float width(domain::UTF8String string) override;

  private:
    std::map<domain::Codepoint, Glyph> m_glyphs;
    domain::FontEngine *m_font_engine;
};

} // namespace opengl

#endif
