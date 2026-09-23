#include "mock_glyphresolver.hpp"
#include "domain/geometry.hpp"
#include "domain/ports/font.hpp"

GlyphResolverMock::GlyphResolverMock(long mock_advance)
    : domain::GlyphResolver(nullptr, nullptr), m_advance(mock_advance)
{
}

const domain::GlyphMetrics &GlyphResolverMock::resolveGlyph(domain::Codepoint c)
{
    auto it = m_glyphs_cache.find(c);
    if (it == m_glyphs_cache.end())
    {
        it = m_glyphs_cache
                 .emplace(c,
                          domain::GlyphMetrics{
                              .codepoint = c,
                              .size = domain::UIVec2{(unsigned int)m_advance, (unsigned int)m_advance},
                              .bearing = domain::IVec2{0, 0},
                              .advance = m_advance,
                          })
                 .first;
    }

    return it->second;
}
