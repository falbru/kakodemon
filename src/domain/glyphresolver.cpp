#include "glyphresolver.hpp"
#include "domain/fontmanager.hpp"

namespace domain
{

GlyphResolver::GlyphResolver(Font *primary_font, FontManager *font_manager)
    : m_primary_font(primary_font), m_font_manager(font_manager)
{
}

const GlyphMetrics &GlyphResolver::resolveGlyph(Codepoint c)
{
    return m_font_manager->getGlyph(c, m_primary_font);
}

GlyphWithFont GlyphResolver::resolveGlyphWithFont(Codepoint c)
{
    return m_font_manager->getGlyphWithFont(c, m_primary_font);
}

} // namespace domain
