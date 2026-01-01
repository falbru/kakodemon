#ifndef DOMAIN_FONTENGINE_HPP_INCLUDED
#define DOMAIN_FONTENGINE_HPP_INCLUDED

#include "domain/ports/font.hpp"
#include "domain/utf8string.hpp"
#include <optional>

namespace domain
{

enum class PixelFormat
{
    GRAYSCALE,
    BGRA
};

struct RasterizedGlyph
{
    GlyphMetrics metrics;
    unsigned int width;
    unsigned int height;
    unsigned char *bitmap;
    PixelFormat format;
};

class FontEngine
{
  public:
    virtual ~FontEngine() = default;

    virtual bool hasGlyph(Codepoint c) const = 0;
    virtual std::optional<RasterizedGlyph> rasterizeGlyph(Codepoint c) = 0;
    virtual std::optional<RasterizedGlyph> rasterizeFallbackGlyph() = 0;

    virtual float getAscender() const = 0;
    virtual float getLineHeight() const = 0;
    virtual int getSize() const = 0;

  protected:
    FontEngine() = default;
};

} // namespace domain

#endif
