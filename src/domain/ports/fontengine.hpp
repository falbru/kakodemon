#ifndef FONTENGINE_HPP_INCLUDED
#define FONTENGINE_HPP_INCLUDED

#include "domain/ports/font.hpp"
#include "domain/utf8string.hpp"
#include <optional>

struct RasterizedGlyph
{
    GlyphMetrics metrics;
    unsigned int width;
    unsigned int height;
    unsigned char *bitmap;
};

class FontEngine
{
  public:
    virtual ~FontEngine() = default;

    virtual bool hasGlyph(Codepoint c) const = 0;
    virtual std::optional<RasterizedGlyph> rasterizeGlyph(Codepoint c) = 0;

    virtual float getAscender() const = 0;
    virtual float getLineHeight() const = 0;

  protected:
    FontEngine() = default;
};

#endif
