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

void GlyphLine::truncate(float max_width, Font *font, FontManager *font_manager) {
    if (width() <= max_width) return;

    auto ellipsis_with_font = font_manager->getGlyphWithFont(0x2026, font);
    const GlyphMetrics& ellipsis = ellipsis_with_font.glyph;

    if (ellipsis.advance > max_width) {
        m_atoms = {};
        return;
    }

    std::vector<GlyphAtom> new_atoms;
    float current_width = 0;
    bool done = false;

    for (const auto& atom : m_atoms) {
        if (done) break;

        std::vector<GlyphRun> new_runs;
        for (const auto& run : atom.getRuns()) {
            std::vector<GlyphMetrics> new_glyphs;
            for (const auto& glyph : run.glyphs) {
                if (current_width + glyph.advance + ellipsis.advance > max_width) {
                    done = true;
                    break;
                }
                new_glyphs.push_back(glyph);
                current_width += glyph.advance;
            }

            if (!new_glyphs.empty()) {
                new_runs.push_back({new_glyphs, run.font});
            }

            if (done) {
                if (new_runs.empty() && new_atoms.size() > 0) {
                    auto prev_runs = new_atoms.back().getRuns();
                    prev_runs.push_back({std::vector<GlyphMetrics>{ellipsis}, ellipsis_with_font.font});
                    new_atoms[new_atoms.size() - 1] = GlyphAtom(prev_runs, new_atoms.back().getFace());
                }else {
                    new_runs.push_back({std::vector<GlyphMetrics>{ellipsis}, ellipsis_with_font.font});
                }
                break;
            }
        }

        if (!new_runs.empty()) {
            new_atoms.push_back(GlyphAtom(new_runs, atom.getFace()));
        }
    }

    m_atoms = new_atoms;
}

};
