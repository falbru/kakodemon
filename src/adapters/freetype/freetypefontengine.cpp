#include "freetypefontengine.hpp"
#include "domain/ports/fontengine.hpp"
#include "spdlog/spdlog.h"
#include <freetype/freetype.h>

FreeTypeFontEngine::FreeTypeFontEngine(std::shared_ptr<FreeTypeLibrary> library, const std::string &path, int size)
    : m_library(library), m_requested_size(size), m_scale(1.0f)
{
    if (FT_New_Face(m_library->get(), path.c_str(), 0, &m_face))
    {
        spdlog::error("FreeType: Failed to load font for path {}", path);
    }

    if (FT_Set_Pixel_Sizes(m_face, 0, size))
    {
        FT_Select_Size(m_face, 0);
    }

    if (m_face->face_flags & FT_FACE_FLAG_COLOR && m_face->size->metrics.y_ppem > 0) {
        m_scale = static_cast<float>(m_requested_size) / m_face->size->metrics.y_ppem;
    }

    m_ascender = (m_face->size->metrics.ascender >> 6) * m_scale;
    m_line_height = (m_face->size->metrics.height >> 6) * m_scale;
}

FreeTypeFontEngine::~FreeTypeFontEngine()
{
    FT_Done_Face(m_face);
}

bool FreeTypeFontEngine::hasGlyph(domain::Codepoint c) const {
    return m_glyphs.find(c) != m_glyphs.end();
}

std::optional<domain::RasterizedGlyph> FreeTypeFontEngine::rasterizeGlyph(domain::Codepoint c) {
    if (hasGlyph(c)) {
        return m_glyphs.at(c);
    }

    FT_UInt glyph_index = FT_Get_Char_Index(m_face, c);
    if (glyph_index == 0) {
        spdlog::warn("FreeType: Glyph does not exist for codepoint {} (U+{:04X})", c, c);
        return std::nullopt;
    }

    domain::PixelFormat format = domain::PixelFormat::GRAYSCALE;

    int load_flags = FT_LOAD_DEFAULT;
    if (m_face->face_flags & FT_FACE_FLAG_COLOR) {
        load_flags = FT_LOAD_COLOR;
        format = domain::PixelFormat::BGRA;
    }
    if (FT_Load_Glyph(m_face, glyph_index, load_flags))
    {
        spdlog::error("FreeType: Failed to load glyph for codepoint {}", c);
        return std::nullopt;
    }

    if (FT_Render_Glyph(m_face->glyph, FT_RENDER_MODE_NORMAL))
    {
        spdlog::error("FreeType: Failed to rasterize glyph for codepoint {}", c);
        return std::nullopt;
    }

    domain::RasterizedGlyph glyph = {
        .metrics = {
            .codepoint = c,
            .size = {
                static_cast<unsigned int>(m_face->glyph->bitmap.width * m_scale),
                static_cast<unsigned int>(m_face->glyph->bitmap.rows * m_scale)
            },
            .bearing = {
                static_cast<int>(m_face->glyph->bitmap_left * m_scale),
                static_cast<int>(m_face->glyph->bitmap_top * m_scale)
            },
            .advance = static_cast<long>(m_face->glyph->advance.x * m_scale) >> 6,
        },
        .width = m_face->glyph->bitmap.width,
        .height = m_face->glyph->bitmap.rows,
        .bitmap = m_face->glyph->bitmap.buffer,
        .format = format
    };

    m_glyphs.insert({c, glyph});

    return glyph;
}

std::optional<domain::RasterizedGlyph> FreeTypeFontEngine::rasterizeFallbackGlyph() {
    if (fallback_glyph.has_value()) {
        return fallback_glyph.value();
    }

    domain::PixelFormat format = domain::PixelFormat::GRAYSCALE;

    int load_flags = FT_LOAD_DEFAULT;
    if (m_face->face_flags & FT_FACE_FLAG_COLOR) {
        load_flags = FT_LOAD_COLOR;
        format = domain::PixelFormat::BGRA;
    }
    if (FT_Load_Glyph(m_face, 0, load_flags))
    {
        return std::nullopt;
    }

    if (FT_Render_Glyph(m_face->glyph, FT_RENDER_MODE_NORMAL))
    {
        return std::nullopt;
    }

    domain::RasterizedGlyph glyph = {
        .metrics = {
            .codepoint = 0,
            .size = {
                static_cast<unsigned int>(m_face->glyph->bitmap.width * m_scale),
                static_cast<unsigned int>(m_face->glyph->bitmap.rows * m_scale)
            },
            .bearing = {
                static_cast<int>(m_face->glyph->bitmap_left * m_scale),
                static_cast<int>(m_face->glyph->bitmap_top * m_scale)
            },
            .advance = static_cast<long>(m_face->glyph->advance.x * m_scale) >> 6,
        },
        .width = m_face->glyph->bitmap.width,
        .height = m_face->glyph->bitmap.rows,
        .bitmap = m_face->glyph->bitmap.buffer,
        .format = format
    };

    fallback_glyph = glyph;

    return glyph;
}

float FreeTypeFontEngine::getAscender() const {
    return m_ascender;
}

float FreeTypeFontEngine::getLineHeight() const {
    return m_line_height;
}

int FreeTypeFontEngine::getSize() const {
    return m_requested_size;
}

FontEngineFactory FreeTypeFontEngine::createFactory(std::shared_ptr<FreeTypeLibrary> library) {
    return [library](const domain::FontMatch &match) -> std::unique_ptr<domain::FontEngine> {
        return std::make_unique<FreeTypeFontEngine>(library, match.path, match.size);
    };
}
