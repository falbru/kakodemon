#ifndef DOMAIN_FONT_HPP_INCLUDED
#define DOMAIN_FONT_HPP_INCLUDED

#include "domain/geometry.hpp"
#include "domain/utf8string.hpp"

namespace domain
{

struct GlyphMetrics
{
    Codepoint codepoint;
    UIVec2 size;
    IVec2 bearing;
    long advance;

    float width() const;
};

class Font
{
  public:
    virtual ~Font() = default;

    virtual bool hasGlyph(Codepoint c) const = 0;
    virtual const GlyphMetrics &getGlyphMetrics(Codepoint c) const = 0;

    virtual void loadGlyph(Codepoint c) = 0;
    virtual const GlyphMetrics &ensureGlyph(Codepoint c) = 0;

    virtual float getAscender() const = 0;
    virtual float getLineHeight() const = 0;

    virtual float width(UTF8String string) = 0;
};

} // namespace domain

#endif
