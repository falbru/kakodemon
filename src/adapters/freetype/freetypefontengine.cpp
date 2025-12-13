#include "freetypefontengine.hpp"
#include "domain/ports/fontengine.hpp"
#include "spdlog/spdlog.h"

FreeTypeFontEngine::FreeTypeFontEngine(const std::string path, int size) {
    if (FT_Init_FreeType(&m_library))
    {
        spdlog::error("FreeType: Could not init FreeType Library");
    }

    if (FT_New_Face(m_library, path.c_str(), 0, &m_face))
    {
        spdlog::error("FreeType: Failed to load font for path {}", path);
    }

    FT_Set_Pixel_Sizes(m_face, 0, size);

    m_ascender = m_face->size->metrics.ascender >> 6;
    m_line_height = m_face->size->metrics.height >> 6;
}

FreeTypeFontEngine::~FreeTypeFontEngine() {
    FT_Done_Face(m_face);
    FT_Done_FreeType(m_library);
}

bool FreeTypeFontEngine::hasGlyph(domain::Codepoint c) const {
    return m_glyphs.find(c) != m_glyphs.end();
}

std::optional<domain::RasterizedGlyph> FreeTypeFontEngine::rasterizeGlyph(domain::Codepoint c) {
    if (hasGlyph(c)) {
        return m_glyphs.at(c);
    }

    if (FT_Load_Char(m_face, c, FT_LOAD_RENDER))
    {
        spdlog::error("FreeType: Failed to load glyph for codepoint {}", c);
        return std::nullopt;
    }

    domain::RasterizedGlyph glyph = {
        .metrics = {
            .codepoint = c,
            .size = { m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows },
            .bearing = { m_face->glyph->bitmap_left, m_face->glyph->bitmap_top },
            .advance = m_face->glyph->advance.x,
        },
        .width = m_face->glyph->bitmap.width,
        .height = m_face->glyph->bitmap.rows,
        .bitmap = m_face->glyph->bitmap.buffer,
    };

    m_glyphs.insert({c, glyph});

    return glyph;
}

float FreeTypeFontEngine::getAscender() const {
    return m_ascender;
}

float FreeTypeFontEngine::getLineHeight() const {
    return m_line_height;
}
