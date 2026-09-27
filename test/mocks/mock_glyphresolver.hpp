#ifndef MOCK_GLYPHRESOLVER_HPP_INCLUDED
#define MOCK_GLYPHRESOLVER_HPP_INCLUDED

#include "domain/codepointstring.hpp"
#include "domain/fontmanager.hpp"
#include "domain/glyphresolver.hpp"
#include "domain/ports/font.hpp"

class GlyphResolverMock : public domain::GlyphResolver
{
  public:
    GlyphResolverMock(long mock_advance, domain::Font *primary_font);

    const domain::GlyphMetrics &resolveGlyph(domain::Codepoint c) override;
    domain::GlyphWithFont resolveGlyphWithFont(domain::Codepoint c) override;

    GlyphResolverMock &withResolveFont(std::function<domain::Font *(domain::Codepoint c)> resolve_font);

  private:
    long m_advance;
    domain::Font *m_primary_font;

    std::map<domain::Codepoint, domain::GlyphMetrics> m_glyphs_cache;
    std::optional<std::function<domain::Font *(domain::Codepoint c)>> m_resolve_font;
};

#endif
