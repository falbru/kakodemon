#include "domain/ports/fontresolver.hpp"
#include "domain/utf8string.hpp"
#include "spdlog/spdlog.h"
#include "fontconfigresolver.hpp"
#include <fontconfig/fontconfig.h>
#include <optional>
#include <string>

FontconfigResolver::FontconfigResolver()
{
    if (!FcInit())
    {
        throw std::runtime_error("Failed to initialize fontconfig");
    }
}

FontconfigResolver::~FontconfigResolver()
{
    FcFini();
}

domain::FontMatch FontconfigResolver::resolve(const std::string& pattern)
{
    FcConfig *config = FcConfigGetCurrent();

    std::string family_name;
    std::optional<int> size;

    size_t last_space = pattern.rfind(' ');
    if (last_space != std::string::npos)
    {
        std::string size_str = pattern.substr(last_space + 1);
        try
        {
            size_t pos;
            int parsed_size = std::stoi(size_str, &pos);
            if (pos == size_str.length() && parsed_size > 0)
            {
                family_name = pattern.substr(0, last_space);
                size = parsed_size;
            }
            else
            {
                family_name = pattern;
            }
        }
        catch (const std::exception&)
        {
            family_name = pattern;
        }
    }
    else
    {
        family_name = pattern;
    }

    std::string fc_pattern = family_name;
    if (size.has_value())
    {
        fc_pattern += ":size=" + std::to_string(size.value());
    }

    FcPattern *pat = FcNameParse(reinterpret_cast<const FcChar8*>(fc_pattern.c_str()));
    if (!pat)
    {
        spdlog::error("Failed to parse font pattern: {}", pattern);
        return domain::FontMatch{"", 0};
    }

    FcConfigSubstitute(config, pat, FcMatchPattern);
    FcDefaultSubstitute(pat);

    FcResult result;
    FcPattern *font = FcFontMatch(config, pat, &result);

    domain::FontMatch match{"", 0, 0};

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

        int face_index = 0;
        if (FcPatternGetInteger(font, FC_INDEX, 0, &face_index) == FcResultMatch)
        {
            match.face_index = face_index;
        }

        FcPatternDestroy(font);
    }
    else
    {
        spdlog::warn("Failed to match font pattern: {}", pattern);
    }

    FcPatternDestroy(pat);

    return match;
}

std::optional<domain::FontMatch> FontconfigResolver::resolveStyleVariant(const domain::FontMatch &base_font, domain::FontStyle style)
{
    FcConfig *config = FcConfigGetCurrent();

    FcPattern *base_pattern = FcFreeTypeQuery(
        reinterpret_cast<const FcChar8*>(base_font.path.c_str()),
        0, nullptr, nullptr
    );

    if (!base_pattern)
    {
        spdlog::error("Failed to query font file: {}", base_font.path);
        return std::nullopt;
    }

    FcChar8 *family = nullptr;
    if (FcPatternGetString(base_pattern, FC_FAMILY, 0, &family) != FcResultMatch)
    {
        spdlog::error("Failed to get family name from font: {}", base_font.path);
        FcPatternDestroy(base_pattern);
        return std::nullopt;
    }

    FcPattern *pat = FcPatternCreate();
    if (!pat)
    {
        spdlog::error("Failed to create font pattern");
        FcPatternDestroy(base_pattern);
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

            int face_index = 0;
            if (FcPatternGetInteger(font, FC_INDEX, 0, &face_index) == FcResultMatch)
            {
                font_match.face_index = face_index;
            }
            else
            {
                font_match.face_index = 0;
            }

            spdlog::debug("Resolved style variant for '{}' to: {} (face index: {})", base_font.path, font_match.path, font_match.face_index);
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

    return match;
}

std::optional<domain::FontMatch> FontconfigResolver::resolveForCodepoint(domain::Codepoint codepoint)
{
    FcConfig *config = FcConfigGetCurrent();

    FcPattern *pat = FcPatternCreate();
    if (!pat)
    {
        spdlog::error("Failed to create font pattern");
        return std::nullopt;
    }

    FcCharSet* charset = FcCharSetCreate();
    FcCharSetAddChar(charset, codepoint);
    FcPatternAddCharSet(pat, FC_CHARSET, charset);

    if (domain::isEmoji(codepoint))
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

            int face_index = 0;
            if (FcPatternGetInteger(font, FC_INDEX, 0, &face_index) == FcResultMatch)
            {
                fontMatch.face_index = face_index;
            }
            else
            {
                fontMatch.face_index = 0;
            }

            spdlog::debug("Resolved codepoint U+{:X} to font: {}", codepoint, fontMatch.path);
            spdlog::debug("Resolved codepoint U+{:X} to font size: {}", codepoint, fontMatch.size);
            spdlog::debug("Resolved codepoint U+{:X} to font face index: {}", codepoint, fontMatch.face_index);
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

    return match;
}
