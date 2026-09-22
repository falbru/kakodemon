#ifndef TEST_MOCK_FONTMANAGER_HPP_INCLUDED
#define TEST_MOCK_FONTMANAGER_HPP_INCLUDED

#include "domain/fontmanager.hpp"

class FontManagerMock : public domain::FontManager
{
  public:
    FontManagerMock() : domain::FontManager(nullptr, nullptr, nullptr)
    {
    }

    domain::GlyphWithFont getGlyphWithFont(domain::Codepoint c, domain::Font *primary_font) override
    {
        static domain::GlyphMetrics glyph{.codepoint = c, .size = {10, 10}, .bearing = {0, 0}, .advance = 10};
        return {glyph, primary_font};
    }
};

#endif
