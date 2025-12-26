#include "glyphline.hpp"

namespace domain {

GlyphLine::GlyphLine(std::vector<GlyphAtom> atoms) : m_atoms(atoms) { }

Line GlyphLine::toLine() const {
    std::vector<Atom> atoms;
    atoms.reserve(m_atoms.size());

    for (int i = 0; i < m_atoms.size(); i++) {
        atoms.push_back(m_atoms[i].toAtom());
    }

    return Line(atoms);
}

const std::vector<GlyphAtom>& GlyphLine::getGlyphAtoms() const {
    return m_atoms;
}

float GlyphLine::width() const {
    float width = 0;
    for (int i = 0; i < m_atoms.size(); i++) {
        width += m_atoms[i].width();
    }
    return width;
}

float GlyphLine::height() const {
    float max_height = 0;
    for (const auto& atom : m_atoms) {
        for (const auto& run : atom.getRuns()) {
            for (const auto& glyph : run.glyphs) {
                if (glyph.size.y > max_height) {
                    max_height = glyph.size.y;
                }
            }
        }
    }
    return max_height;
}

};
