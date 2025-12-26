#include "font.hpp"
#include "domain/utf8string.hpp"
#include "domain/ports/fontengine.hpp"

#include <spdlog/spdlog.h>

opengl::Font::Font(domain::FontEngine* font_engine)
    : m_font_engine(font_engine) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (domain::Codepoint c = 0x20; c < 128; c++)
    {
        if (c == 0x7F) continue;

        loadGlyph(c);
    }

    if(auto fallback_rasterized_glyph = m_font_engine->rasterizeFallbackGlyph()) {
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        if (fallback_rasterized_glyph->format == domain::PixelFormat::GRAYSCALE) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, fallback_rasterized_glyph->width, fallback_rasterized_glyph->height, 0, GL_RED, GL_UNSIGNED_BYTE, fallback_rasterized_glyph->bitmap);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }else {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fallback_rasterized_glyph->width, fallback_rasterized_glyph->height, 0, GL_BGRA, GL_UNSIGNED_BYTE, fallback_rasterized_glyph->bitmap);
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        fallback_glyph = opengl::Glyph{texture, fallback_rasterized_glyph->metrics, fallback_rasterized_glyph->format};
    }
}

opengl::Font::~Font() {
    for (auto& pair : m_glyphs) {
        glDeleteTextures(1, &pair.second.texture_id);
    }

    if (fallback_glyph.has_value()) {
        glDeleteTextures(1, &fallback_glyph->texture_id);
    }
}

bool opengl::Font::hasGlyph(domain::Codepoint c) const {
    return m_glyphs.find(c) != m_glyphs.end();
}

const domain::GlyphMetrics &opengl::Font::getGlyphMetrics(domain::Codepoint c) const {
    return m_glyphs.at(c).metrics;
}

const domain::GlyphMetrics &opengl::Font::getFallbackGlyphMetrics() const {
    return fallback_glyph->metrics;
}

const opengl::Glyph &opengl::Font::getGlyph(domain::Codepoint c) const {
    return m_glyphs.at(c);
}

bool opengl::Font::loadGlyph(domain::Codepoint c) {
    if (hasGlyph(c)) {
        return true;
    }

    if (auto rasterized_glyph = m_font_engine->rasterizeGlyph(c)) {
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        if (rasterized_glyph->format == domain::PixelFormat::GRAYSCALE) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, rasterized_glyph->width, rasterized_glyph->height, 0, GL_RED, GL_UNSIGNED_BYTE, rasterized_glyph->bitmap);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }else {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rasterized_glyph->width, rasterized_glyph->height, 0, GL_BGRA, GL_UNSIGNED_BYTE, rasterized_glyph->bitmap);
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        Glyph glyph = {texture, rasterized_glyph->metrics, rasterized_glyph->format};

        m_glyphs.insert(std::pair<domain::Codepoint, Glyph>(c, glyph));
        return true;
    }

    return false;
}

float opengl::Font::getAscender() const {
    return m_font_engine->getAscender();
}

float opengl::Font::getLineHeight() const {
    return m_font_engine->getLineHeight();
}
