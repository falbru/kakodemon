#ifndef DOMAIN_FONTMANAGER_HPP_INCLUDED
#define DOMAIN_FONTMANAGER_HPP_INCLUDED

#include "domain/ports/font.hpp"
#include "domain/ports/fontengine.hpp"
#include "domain/ports/fontresolver.hpp"
#include <functional>
#include <map>
#include <memory>
#include <string>

namespace domain
{

struct GlyphWithFont
{
    GlyphMetrics glyph;
    Font *font;
};

class FontManager;

using FontFactory = std::function<std::unique_ptr<Font>(FontEngine *, FontManager *)>;

using FontEngineFactory = std::function<std::unique_ptr<FontEngine>(const FontMatch &)>;

class FontManager
{
  public:
    FontManager(std::unique_ptr<FontResolver> resolver, FontEngineFactory engine_factory, FontFactory font_factory);

    Font *getFontFromName(const std::string &pattern);
    Font *getFontForCodepoint(Codepoint c, Font *primary_font);

    const GlyphMetrics &getGlyph(Codepoint c, Font *primary_font);
    GlyphWithFont getGlyphWithFont(Codepoint c, Font *primary_font);

  private:
    struct CachedFont
    {
        std::unique_ptr<FontEngine> engine;
        std::unique_ptr<Font> font;
    };

    Font *getOrCreateFont(const FontMatch &match);
    std::string makeCacheKey(const FontMatch &match) const;

    std::unique_ptr<FontResolver> m_resolver;
    FontEngineFactory m_engine_factory;
    FontFactory m_font_factory;

    std::map<std::string, CachedFont> m_fonts;
    std::map<Codepoint, std::map<int, Font *>> m_codepoint_cache;
};

} // namespace domain

#endif
