#ifndef TEST_MOCK_FONT_HPP_INCLUDED
#define TEST_MOCK_FONT_HPP_INCLUDED

#include "domain/ports/font.hpp"
#include <map>

class FontMock : public domain::Font
{
  private:
    std::map<domain::Codepoint, domain::GlyphMetrics> metrics_cache;

  public:
    bool hasGlyph(domain::Codepoint cp) const override
    {
        return true;
    }

    const domain::GlyphMetrics &getGlyphMetrics(domain::Codepoint cp) const override
    {
        return metrics_cache.at(cp);
    }

    void loadGlyph(domain::Codepoint cp) override
    {
    }

    const domain::GlyphMetrics &ensureGlyph(domain::Codepoint cp) override
    {
        if (metrics_cache.find(cp) == metrics_cache.end())
        {
            metrics_cache[cp] =
                domain::GlyphMetrics{.codepoint = cp, .size = {10, 10}, .bearing = {0, 0}, .advance = 640};
        }
        return metrics_cache[cp];
    }

    float getAscender() const override
    {
        return 20.0f;
    }

    float getLineHeight() const override
    {
        return 24.0f;
    }

    float width(domain::UTF8String string) override
    {
        return string.size() * 10.0f;
    }
};

#endif
