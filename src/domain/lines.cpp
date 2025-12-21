#include "lines.hpp"
#include "domain/atom.hpp"
#include "domain/line.hpp"
#include "domain/ports/font.hpp"
#include <optional>

namespace domain {

Lines::Lines() {

}

Lines::Lines(std::vector<Line> lines) : m_lines(lines) {

}

const Line& Lines::at(int index) const {
    return m_lines.at(index);
}

const std::vector<Line>& Lines::getLines() const {
    return m_lines;
}

unsigned int Lines::size() const {
    return m_lines.size();
}

GlyphLines::GlyphLines(const Lines& lines, Font* font) : m_line_height(font->getLineHeight()) {
    m_lines.reserve(lines.size());

    for (int i = 0; i < lines.size(); i++) {
        m_lines.push_back(GlyphLine(lines.getLines()[i], font));
    }
}

Lines GlyphLines::toLines() const {
    std::vector<Line> lines(m_lines.size());

    for (int i = 0; i < m_lines.size(); i++) {
        lines[i] = m_lines[i].toLine();
    }

    return Lines(lines);
}

const std::vector<GlyphLine>& GlyphLines::getGlyphLines() const {
    return m_lines;
}

unsigned int GlyphLines::size() const {
    return m_lines.size();
}

float GlyphLines::width() const {
    float biggest_line_width = 0;

    for (int i = 0; i < m_lines.size(); i++) {
        float line_width = m_lines[i].width();
        if (line_width > biggest_line_width) {
            biggest_line_width = line_width;
        }
    }

    return biggest_line_width;
}

float GlyphLines::height() const {
    return m_line_height * m_lines.size();
}

void GlyphLines::wrap(float max_width, WrapMode mode) {
    std::optional<GlyphLine> remainder = std::nullopt;

    std::vector<GlyphLine> wrapped_lines;

    int original_lines_size = m_lines.size();
    int line_index = 0;
    while (line_index < original_lines_size || remainder.has_value()) {
        std::vector<GlyphAtom> wrapped_line;
        float width_it = 0.0f;

        auto atoms = remainder.has_value() ? remainder->getGlyphAtoms() : m_lines[line_index].getGlyphAtoms();

        auto atom_it = atoms.begin();
        while (atom_it != atoms.end()) {
            float atom_width = atom_it->width();
            if (width_it + atom_width > max_width)
                break;
            wrapped_line.push_back(*atom_it);
            width_it += atom_width;
            atom_it++;
        }

        if (atom_it != atoms.end()) {
            std::vector<GlyphMetrics> fitted_glyphs;
            std::vector<GlyphMetrics> remainder_glyphs;
            auto glyphs = atom_it->getGlyphs();
            int last_whitespace_index = -1;
            int glyph_index = 0;

            while (glyph_index < glyphs.size()) {
                float glyph_width = glyphs[glyph_index].width();
                if (width_it + glyph_width > max_width)
                    break;
                if (isWhitespace(glyphs[glyph_index].codepoint)) {
                    last_whitespace_index = glyph_index;
                }
                fitted_glyphs.push_back(glyphs[glyph_index]);
                width_it += glyph_width;
                glyph_index++;
            }

            if (mode == WrapMode::WORD && last_whitespace_index >= 0) {
                size_t split_point = last_whitespace_index;
                if (split_point < fitted_glyphs.size()) {
                    remainder_glyphs.assign(
                        fitted_glyphs.begin() + split_point + 1,
                        fitted_glyphs.end()
                    );
                    fitted_glyphs.erase(
                        fitted_glyphs.begin() + split_point,
                        fitted_glyphs.end()
                    );
                }
            }

            while (glyph_index < glyphs.size()) {
                remainder_glyphs.push_back(glyphs[glyph_index]);
                glyph_index++;
            }

            if (fitted_glyphs.size() > 0) wrapped_line.push_back(GlyphAtom(fitted_glyphs, atom_it->getFace()));

            std::vector<GlyphAtom> remainder_atoms;;

            if (!remainder_glyphs.empty()) {
                remainder_atoms.push_back(GlyphAtom(remainder_glyphs, atom_it->getFace()));
            }

            atom_it++;
            while(atom_it != atoms.end()) {
                remainder_atoms.push_back(*atom_it);
                atom_it++;
            }

            remainder = GlyphLine(remainder_atoms);
        }else {
            remainder = std::nullopt;
            line_index++;
        }

        wrapped_lines.push_back(GlyphLine(wrapped_line));
    }

    m_lines = wrapped_lines;
}

}
