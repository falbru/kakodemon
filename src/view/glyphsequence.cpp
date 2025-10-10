#include "view/glyphsequence.hpp"
#include "core/utf8string.hpp"

GlyphSequence::GlyphSequence(std::shared_ptr<opengl::Font> font, const UTF8String& string) : m_font(font) {
    for (int i = 0; i < string.size(); i++) {
        m_glyphs.push_back(m_font->ensureGlyph(string.at(i)));
    }
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

const std::vector<opengl::Glyph>& GlyphSequence::glyphs() const {
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

UTF8String GlyphSequence::toUTF8String() {
    std::vector<Codepoint> codepoints;
    for (const auto& glyph : m_glyphs) {
        codepoints.push_back(glyph.codepoint);
    }
    return UTF8String(codepoints);
}
