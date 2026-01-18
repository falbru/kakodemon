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
    return match.path + ":" + std::to_string(match.size) + ":" + std::to_string(match.face_index);
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
    m_font_to_match[font_ptr] = match;

    return font_ptr;
}

Font *FontManager::getFontFromName(const std::string &pattern)
{
    auto it = m_name_cache.find(pattern);
    if (it != m_name_cache.end())
    {
        return it->second;
    }

    auto match = m_resolver->resolve(pattern);
    Font *font = getOrCreateFont(match);
    m_name_cache[pattern] = font;
    return font;
}

Font *FontManager::getDefaultFont(int size)
{
    auto it = m_default_font_cache.find(size);
    if (it != m_default_font_cache.end())
    {
        return it->second;
    }

    auto match = m_resolver->resolveDefault(size);
    Font *font = getOrCreateFont(match);
    m_default_font_cache[size] = font;
    return font;
}

Font *FontManager::getFontForCodepoint(Codepoint c, Font *primary_font)
{
    int primary_size = primary_font->getSize();

    auto &size_cache = m_codepoint_cache[c];
    auto it = size_cache.find(primary_size);
    if (it != size_cache.end())
    {
        return it->second;
    }

    auto match_opt = m_resolver->resolveForCodepoint(c);
    if (!match_opt.has_value())
    {
        return nullptr;
    }

    FontMatch match = match_opt.value();
    match.size = primary_size;

    Font *font = getOrCreateFont(match);

    size_cache[primary_size] = font;

    return font;
}

const GlyphMetrics& FontManager::getGlyph(Codepoint c, Font* primary_font)
{
    if (primary_font->loadGlyph(c)) {
        return primary_font->getGlyphMetrics(c);
    }

    auto fallback_font = getFontForCodepoint(c, primary_font);
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

    auto fallback_font = getFontForCodepoint(c, primary_font);
    if (fallback_font == nullptr || !fallback_font->loadGlyph(c)) {
        return GlyphWithFont{primary_font->getFallbackGlyphMetrics(), primary_font};
    }

    return GlyphWithFont{fallback_font->getGlyphMetrics(c), fallback_font};
}

Font *FontManager::getFontStyleVariant(Font *base_font, FontStyle style)
{
    auto &style_cache = m_style_variant_cache[base_font];
    auto it = style_cache.find(style);
    if (it != style_cache.end())
    {
        return it->second;
    }

    auto match_it = m_font_to_match.find(base_font);
    if (match_it == m_font_to_match.end())
    {
        style_cache[style] = nullptr;
        return nullptr;
    }

    const FontMatch &base_match = match_it->second;
    auto variant_match_opt = m_resolver->resolveStyleVariant(base_match, style);

    if (!variant_match_opt.has_value())
    {
        style_cache[style] = nullptr;
        return nullptr;
    }

    Font *variant_font = getOrCreateFont(variant_match_opt.value());
    style_cache[style] = variant_font;

    return variant_font;
}

}
