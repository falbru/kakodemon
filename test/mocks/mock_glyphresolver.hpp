#ifndef MOCK_GLYPHRESOLVER_HPP_INCLUDED
#define MOCK_GLYPHRESOLVER_HPP_INCLUDED

#include "domain/codepointstring.hpp"
#include "domain/glyphresolver.hpp"
#include "domain/ports/font.hpp"

class GlyphResolverMock : public domain::GlyphResolver
{
  public:
    GlyphResolverMock(long mock_advance);

    const domain::GlyphMetrics &resolveGlyph(domain::Codepoint c) override;

  private:
    long m_advance;

    std::map<domain::Codepoint, domain::GlyphMetrics> m_glyphs_cache;
};

#endif
