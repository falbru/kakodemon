#include "domain/fontmanager.hpp"

namespace domain {

FontManager::FontManager(std::unique_ptr<FontResolver> resolver,
                                 FontEngineFactory engine_factory,
                                 FontFactory font_factory)
    : m_resolver(std::move(resolver)),
      m_engine_factory(std::move(engine_factory)),
      m_font_factory(std::move(font_factory))
{
}

std::string FontManager::makeCacheKey(const FontMatch &match) const
{
    return match.path + ":" + std::to_string(match.size);
}

Font *FontManager::getOrCreateFont(const FontMatch &match)
{
    std::string key = makeCacheKey(match);

    auto it = m_fonts.find(key);
    if (it != m_fonts.end())
    {
        return it->second.font.get();
    }

    auto engine = m_engine_factory(match);
    auto font = m_font_factory(engine.get(), this);

    CachedFont cached{std::move(engine), std::move(font)};
    Font *font_ptr = cached.font.get();

    m_fonts[key] = std::move(cached);

    return font_ptr;
}

Font *FontManager::getFontFromName(const std::string &pattern)
{
    auto match = m_resolver->resolve(pattern);
    return getOrCreateFont(match);
}

Font *FontManager::getFontForCodepoint(Codepoint c)
{
    auto it = m_codepoint_cache.find(c);
    if (it != m_codepoint_cache.end())
    {
        return it->second;
    }

    auto match_opt = m_resolver->resolveForCodepoint(c);
    if (!match_opt.has_value())
    {
        return nullptr;
    }

    Font *font = getOrCreateFont(match_opt.value());

    m_codepoint_cache[c] = font;

    return font;
}

const GlyphMetrics& FontManager::getGlyph(Codepoint c, Font* primary_font)
{
    if (primary_font->loadGlyph(c)) {
        return primary_font->getGlyphMetrics(c);
    }

    auto fallback_font = getFontForCodepoint(c);
    if (fallback_font == nullptr || !fallback_font->loadGlyph(c)) {
        return primary_font->getFallbackGlyphMetrics();
    }

    return fallback_font->getGlyphMetrics(c);
}

GlyphWithFont FontManager::getGlyphWithFont(Codepoint c, Font* primary_font)
{
    if (primary_font->loadGlyph(c)) {
        return GlyphWithFont{primary_font->getGlyphMetrics(c), primary_font};
    }

    auto fallback_font = getFontForCodepoint(c);
    if (fallback_font == nullptr || !fallback_font->loadGlyph(c)) {
        return GlyphWithFont{primary_font->getFallbackGlyphMetrics(), primary_font};
    }

    return GlyphWithFont{fallback_font->getGlyphMetrics(c), fallback_font};
}

}
