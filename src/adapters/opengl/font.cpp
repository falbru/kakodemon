#include "font.hpp"
#include "domain/utf8string.hpp"
#include "domain/ports/fontengine.hpp"

#include <spdlog/spdlog.h>

opengl::Font::Font(FontEngine* font_engine) : m_font_engine(font_engine) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (Codepoint c = 0; c < 128; c++)
    {
        loadGlyph(c);
    }
}

opengl::Font::~Font() { }

bool opengl::Font::hasGlyph(Codepoint c) const {
    return m_glyphs.find(c) != m_glyphs.end();
}

const GlyphMetrics &opengl::Font::getGlyphMetrics(Codepoint c) const {
    return m_glyphs.at(c).metrics;
}

const opengl::Glyph &opengl::Font::getGlyph(Codepoint c) const {
    return m_glyphs.at(c);
}

void opengl::Font::loadGlyph(Codepoint c) {
    if (auto rasterized_glyph = m_font_engine->rasterizeGlyph(c)) {
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, rasterized_glyph.value().width, rasterized_glyph.value().height, 0, GL_RED, GL_UNSIGNED_BYTE, rasterized_glyph.value().bitmap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Glyph glyph = {texture, rasterized_glyph.value().metrics};

        m_glyphs.insert(std::pair<Codepoint, Glyph>(c, glyph));
    }
}

const GlyphMetrics& opengl::Font::ensureGlyph(Codepoint c) {
    if (!hasGlyph(c)) {
        loadGlyph(c);
    }

    return getGlyphMetrics(c);
}

float opengl::Font::getAscender() const {
    return m_font_engine->getAscender();
}

float opengl::Font::getLineHeight() const {
    return m_font_engine->getLineHeight();
}

float opengl::Font::width(UTF8String string) {
    float w = 0;

    for (int i = 0; i < string.size(); i++) {
        if (hasGlyph(string.at(i))) {
            w += ensureGlyph(string.at(i)).width();
        }
    }

    return w;
}
