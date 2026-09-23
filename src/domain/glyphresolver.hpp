#ifndef GLYPHRESOLVER_HPP_INCLUDED
#define GLYPHRESOLVER_HPP_INCLUDED

#include "domain/fontmanager.hpp"

namespace domain
{

class GlyphResolver
{
  public:
    GlyphResolver(Font *primary_font, FontManager *font_manager);

    virtual const GlyphMetrics &resolveGlyph(Codepoint c);

  private:
    Font *m_primary_font;
    FontManager *m_font_manager;
};

} // namespace domain

#endif
