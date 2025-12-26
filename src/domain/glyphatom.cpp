#include "glyphatom.hpp"

namespace domain{

GlyphAtom::GlyphAtom(const std::vector<GlyphRun>& runs, const Face& face) : m_runs(runs), m_face(face) {

}

Atom GlyphAtom::toAtom() const {
    std::vector<Codepoint> codepoints;

    for (const auto& run : m_runs) {
        for (const auto& glyph : run.glyphs) {
            codepoints.push_back(glyph.codepoint);
        }
    }

    return Atom(codepoints, m_face);
}

const std::vector<GlyphRun>& GlyphAtom::getRuns() const {
    return m_runs;
}

const Face& GlyphAtom::getFace() const {
    return m_face;
}

float GlyphAtom::width() const {
    float width = 0;

    for (const auto& run : m_runs) {
        for (const auto& glyph : run.glyphs) {
            width += glyph.advance;
        }
    }

    return width;
}

}
