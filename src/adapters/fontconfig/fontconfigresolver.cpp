#include "domain/ports/fontresolver.hpp"
#include "spdlog/spdlog.h"
#include "fontconfigresolver.hpp"
#include <fontconfig/fontconfig.h>

FontconfigResolver::FontconfigResolver()
{
    if (!FcInit())
    {
        spdlog::error("Failed to initialize fontconfig");
    }
}

FontconfigResolver::~FontconfigResolver()
{
    FcFini();
}

domain::FontMatch FontconfigResolver::resolve(const std::string& pattern)
{
    FcConfig *config = FcInitLoadConfigAndFonts();
    if (!config)
    {
        spdlog::error("Failed to load fontconfig configuration");
        return domain::FontMatch{"", 0};
    }

    FcPattern *pat = FcNameParse(reinterpret_cast<const FcChar8*>(pattern.c_str()));
    if (!pat)
    {
        spdlog::error("Failed to parse font pattern: {}", pattern);
        FcConfigDestroy(config);
        return domain::FontMatch{"", 0};
    }

    FcConfigSubstitute(config, pat, FcMatchPattern);
    FcDefaultSubstitute(pat);

    FcResult result;
    FcPattern *font = FcFontMatch(config, pat, &result);

    domain::FontMatch match{"", 0};

    if (font && result == FcResultMatch)
    {
        FcChar8 *file = nullptr;
        if (FcPatternGetString(font, FC_FILE, 0, &file) == FcResultMatch)
        {
            match.path = reinterpret_cast<char*>(file);
            spdlog::debug("Resolved font pattern '{}' to: {}", pattern, match.path);
        }

        double size = 0;
        if (FcPatternGetDouble(font, FC_SIZE, 0, &size) == FcResultMatch)
        {
            match.size = static_cast<int>(size);
        }
        else if (FcPatternGetDouble(pat, FC_SIZE, 0, &size) == FcResultMatch)
        {
            match.size = static_cast<int>(size);
        }

        FcPatternDestroy(font);
    }
    else
    {
        spdlog::warn("Failed to match font pattern: {}", pattern);
    }

    FcPatternDestroy(pat);
    FcConfigDestroy(config);

    return match;
}

std::optional<domain::FontMatch> FontconfigResolver::resolveStyleVariant(const domain::FontMatch &base_font, domain::FontStyle style)
{
    FcConfig *config = FcInitLoadConfigAndFonts();
    if (!config)
    {
        spdlog::error("Failed to load fontconfig configuration");
        return std::nullopt;
    }

    FcPattern *base_pattern = FcFreeTypeQuery(
        reinterpret_cast<const FcChar8*>(base_font.path.c_str()),
        0, nullptr, nullptr
    );

    if (!base_pattern)
    {
        spdlog::error("Failed to query font file: {}", base_font.path);
        FcConfigDestroy(config);
        return std::nullopt;
    }

    FcChar8 *family = nullptr;
    if (FcPatternGetString(base_pattern, FC_FAMILY, 0, &family) != FcResultMatch)
    {
        spdlog::error("Failed to get family name from font: {}", base_font.path);
        FcPatternDestroy(base_pattern);
        FcConfigDestroy(config);
        return std::nullopt;
    }

    FcPattern *pat = FcPatternCreate();
    if (!pat)
    {
        spdlog::error("Failed to create font pattern");
        FcPatternDestroy(base_pattern);
        FcConfigDestroy(config);
        return std::nullopt;
    }

    FcPatternAddString(pat, FC_FAMILY, family);
    FcPatternAddDouble(pat, FC_SIZE, static_cast<double>(base_font.size));

    int weight = FC_WEIGHT_REGULAR;
    int slant = FC_SLANT_ROMAN;

    switch (style)
    {
        case domain::FontStyle::Normal:
            weight = FC_WEIGHT_REGULAR;
            slant = FC_SLANT_ROMAN;
            break;
        case domain::FontStyle::Bold:
            weight = FC_WEIGHT_BOLD;
            slant = FC_SLANT_ROMAN;
            break;
        case domain::FontStyle::Italic:
            weight = FC_WEIGHT_REGULAR;
            slant = FC_SLANT_ITALIC;
            break;
        case domain::FontStyle::BoldItalic:
            weight = FC_WEIGHT_BOLD;
            slant = FC_SLANT_ITALIC;
            break;
    }

    FcPatternAddInteger(pat, FC_WEIGHT, weight);
    FcPatternAddInteger(pat, FC_SLANT, slant);

    FcConfigSubstitute(config, pat, FcMatchPattern);
    FcDefaultSubstitute(pat);

    FcResult result;
    FcPattern *font = FcFontMatch(config, pat, &result);

    std::optional<domain::FontMatch> match;

    if (font && result == FcResultMatch)
    {
        FcChar8 *file = nullptr;
        if (FcPatternGetString(font, FC_FILE, 0, &file) == FcResultMatch)
        {
            domain::FontMatch font_match;
            font_match.path = reinterpret_cast<char*>(file);
            font_match.size = base_font.size;

            spdlog::debug("Resolved style variant for '{}' to: {}", base_font.path, font_match.path);
            match = font_match;
        }

        FcPatternDestroy(font);
    }
    else
    {
        spdlog::warn("Failed to find style variant for font: {}", base_font.path);
    }

    FcPatternDestroy(pat);
    FcPatternDestroy(base_pattern);
    FcConfigDestroy(config);

    return match;
}

std::optional<domain::FontMatch> FontconfigResolver::resolveForCodepoint(domain::Codepoint codepoint)
{
    FcConfig *config = FcInitLoadConfigAndFonts();
    if (!config)
    {
        spdlog::error("Failed to load fontconfig configuration");
        return std::nullopt;
    }

    FcPattern *pat = FcPatternCreate();
    if (!pat)
    {
        spdlog::error("Failed to create font pattern");
        FcConfigDestroy(config);
        return std::nullopt;
    }

    FcCharSet* charset = FcCharSetCreate();
    FcCharSetAddChar(charset, codepoint);
    FcPatternAddCharSet(pat, FC_CHARSET, charset);

    if (codepoint >= 0x1F300)
    {
        FcPatternAddBool(pat, FC_COLOR, FcTrue);
    }

    FcConfigSubstitute(config, pat, FcMatchPattern);
    FcDefaultSubstitute(pat);

    FcResult result;
    FcPattern *font = FcFontMatch(config, pat, &result);

    std::optional<domain::FontMatch> match;

    if (font && result == FcResultMatch)
    {
        FcChar8 *file = nullptr;
        if (FcPatternGetString(font, FC_FILE, 0, &file) == FcResultMatch)
        {
            domain::FontMatch fontMatch;
            fontMatch.path = reinterpret_cast<char*>(file);

            double size = 0;
            if (FcPatternGetDouble(font, FC_SIZE, 0, &size) == FcResultMatch)
            {
                fontMatch.size = static_cast<int>(size);
            }

            spdlog::info("Resolved codepoint U+{:X} to font: {}", codepoint, fontMatch.path);
            spdlog::info("Resolved codepoint U+{:X} to font size: {}", codepoint, fontMatch.size);
            match = fontMatch;
        }

        FcPatternDestroy(font);
    }
    else
    {
        spdlog::warn("Failed to find font for codepoint U+{:X}", codepoint);
    }

    FcCharSetDestroy(charset);
    FcPatternDestroy(pat);
    FcConfigDestroy(config);

    return match;
}
