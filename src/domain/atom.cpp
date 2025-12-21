#include "atom.hpp"
#include "domain/ports/font.hpp"
#include "domain/utf8string.hpp"

namespace domain {

Atom::Atom(UTF8String contents, Face face) : m_contents(contents), m_face(face) {

}

const UTF8String& Atom::getContents() const {
    return m_contents;
}

const Face& Atom::getFace() const {
    return m_face;
}

unsigned int Atom::size() const {
    return m_contents.size();
}

Atom Atom::slice(int start_index) {
    return slice(start_index, size() - start_index);
}

Atom Atom::slice(int start_index, int length) {
    if (start_index < 0 || start_index >= m_contents.size() || length < 0)
        return Atom({}, m_face);

    return Atom(m_contents.substring(start_index, length), m_face);
}

UTF8String Atom::toUTF8String() const {
    return UTF8String(m_contents);
}

GlyphAtom::GlyphAtom(const std::vector<GlyphMetrics>& glyphs, const Face& face) : m_glyphs(glyphs), m_face(face) {

}

GlyphAtom::GlyphAtom(const Atom& atom, Font* font) : m_face(atom.getFace()) {
    auto contents = atom.getContents();

    m_glyphs = std::vector<GlyphMetrics>(contents.size());
    for (int i = 0; i < contents.size(); i++) {
        m_glyphs[i] = font->ensureGlyph(contents.at(i));
    }
}

Atom GlyphAtom::toAtom() const {
    std::vector<Codepoint> codepoints(m_glyphs.size());

    for (int i = 0; i < m_glyphs.size(); i++) {
        codepoints[i] = m_glyphs[i].codepoint;
    }

    return Atom(codepoints, m_face);
}

const std::vector<GlyphMetrics>& GlyphAtom::getGlyphs() const {
    return m_glyphs;
}

const Face& GlyphAtom::getFace() const {
    return m_face;
}

float GlyphAtom::width() const {
    float width = 0;

    for (int i = 0; i < m_glyphs.size(); i++) {
        width += m_glyphs[i].width();
    }

    return width;
}

}
