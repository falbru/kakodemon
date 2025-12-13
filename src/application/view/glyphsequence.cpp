#include "glyphsequence.hpp"
#include "domain/utf8string.hpp"
#include "domain/ports/font.hpp"

GlyphSequence::GlyphSequence(domain::Font* font, const domain::UTF8String& string) : m_font(font) {
    for (int i = 0; i < string.size(); i++) {
        m_glyphs.push_back(m_font->ensureGlyph(string.at(i)));
    }
}

GlyphSequence::GlyphSequence(domain::Font* font, const std::vector<domain::GlyphMetrics> &glyphs) : m_font(font), m_glyphs(glyphs) {
}

float GlyphSequence::width() const {
    float w = 0;
    for (auto glyph : m_glyphs) {
        w += glyph.width();
    }
    return w;
}

float GlyphSequence::count() const {
    return m_glyphs.size();

}

const std::vector<domain::GlyphMetrics>& GlyphSequence::glyphs() const {
    return m_glyphs;

}

void GlyphSequence::truncate(float max_width) {
    if (width() < max_width) return;

    auto ellipsis = m_font->ensureGlyph(0x2026);

    auto it = m_glyphs.begin();
    float it_width = 0;
    while (it != m_glyphs.end() && it_width + it->width() + ellipsis.width() < max_width) {
        it_width += it->width();
        it++;
    }

    m_glyphs.erase(it, m_glyphs.end());
    m_glyphs.push_back(ellipsis);
}

GlyphSequence GlyphSequence::substr(int start, int length) {
    return GlyphSequence(m_font, std::vector<domain::GlyphMetrics>(m_glyphs.begin() + start, m_glyphs.begin() + start + length));
}

domain::UTF8String GlyphSequence::toUTF8String() {
    std::vector<domain::Codepoint> codepoints;
    for (const auto& glyph : m_glyphs) {
        codepoints.push_back(glyph.codepoint);
    }
    return domain::UTF8String(codepoints);
}

std::optional<GlyphSequence> GlyphSequence::cut(float max_width, CutMode mode) {
    if (width() <= max_width) {
        return std::nullopt;
    }

    auto it = m_glyphs.begin();
    float it_width = 0;
    while (it != m_glyphs.end() && it_width + it->width() <= max_width) {
        it_width += it->width();
        it++;
    }

    if (mode == CutMode::WORD && it != m_glyphs.end()) {
        while (it != m_glyphs.begin() && it->codepoint != 0x20) {
            it--;
        }
        if (it != m_glyphs.begin() && it->codepoint == 0x20) {
            it++;
        }
    }

    std::vector<domain::GlyphMetrics> glyphs;
    glyphs.assign(it, m_glyphs.end());
    GlyphSequence remaining(m_font, glyphs);

    m_glyphs.erase(it, m_glyphs.end());

    return remaining;
}
