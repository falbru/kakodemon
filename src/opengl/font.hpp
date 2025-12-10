#ifndef OPENGL_FONT_HPP_INCLUDED
#define OPENGL_FONT_HPP_INCLUDED

#include "../font.hpp"
#include "core/utf8string.hpp"
#include "fontengine.hpp"
#include "opengl.hpp"
#include <map>

namespace opengl
{

struct Glyph
{
    unsigned int texture_id;
    GlyphMetrics metrics;
};

class Font : public ::Font
{
  public:
    Font(FontEngine *font_engine);
    virtual ~Font();

    bool hasGlyph(Codepoint c) const override;
    const GlyphMetrics &getGlyphMetrics(Codepoint c) const override;

    const Glyph &getGlyph(Codepoint c) const;

    void loadGlyph(Codepoint c) override;
    const GlyphMetrics &ensureGlyph(Codepoint c) override;

    float getAscender() const override;
    float getLineHeight() const override;

    float width(UTF8String string) override;

  private:
    std::map<Codepoint, Glyph> m_glyphs;
    FontEngine *m_font_engine;
};

} // namespace opengl

#endif
