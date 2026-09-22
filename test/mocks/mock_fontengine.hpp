#ifndef TEST_MOCK_FONTENGINE_HPP_INCLUDED
#define TEST_MOCK_FONTENGINE_HPP_INCLUDED

#include "domain/ports/fontengine.hpp"
#include <optional>

class FontEngineMock : public domain::FontEngine
{
  public:
    bool hasGlyph(domain::Codepoint) const override
    {
        return true;
    }

    std::optional<domain::RasterizedGlyph> rasterizeGlyph(domain::Codepoint) override
    {
        return std::nullopt;
    }

    std::optional<domain::RasterizedGlyph> rasterizeFallbackGlyph() override
    {
        return std::nullopt;
    }

    float getAscender() const override
    {
        return 20.0f;
    }

    float getDescender() const override
    {
        return -4.0f;
    }

    float getLineHeight() const override
    {
        return 24.0f;
    }

    float getUnderlineOffset() const override
    {
        return 0.0f;
    }

    float getUnderlineThickness() const override
    {
        return 1.0f;
    }

    int getSize() const override
    {
        return 14;
    }
};

#endif
