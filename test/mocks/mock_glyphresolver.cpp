#include "mock_glyphresolver.hpp"
#include "domain/fontmanager.hpp"
#include "domain/geometry.hpp"
#include "domain/ports/font.hpp"

GlyphResolverMock::GlyphResolverMock(long mock_advance, domain::Font *primary_font)
    : domain::GlyphResolver(nullptr, nullptr), m_advance(mock_advance), m_primary_font(primary_font)
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

domain::GlyphWithFont GlyphResolverMock::resolveGlyphWithFont(domain::Codepoint c)
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

    domain::Font *font = m_primary_font;
    if (m_resolve_font.has_value())
    {
        font = m_resolve_font.value()(c);
    }

    return domain::GlyphWithFont{it->second, font};
}

GlyphResolverMock &GlyphResolverMock::withResolveFont(std::function<domain::Font *(domain::Codepoint c)> resolve_font)
{
    m_resolve_font = resolve_font;
    return *this;
}
