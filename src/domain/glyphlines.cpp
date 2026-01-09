#include "glyphlines.hpp"
#include <optional>

namespace domain {

GlyphLines::GlyphLines(const std::vector<GlyphLine>& lines, float line_height) : m_lines(lines), m_line_height(line_height) { }

Lines GlyphLines::toLines() const {
    std::vector<Line> lines;
    lines.reserve(m_lines.size());

    for (int i = 0; i < m_lines.size(); i++) {
        lines.push_back(m_lines[i].toLine());
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
            std::vector<GlyphRun> fitted_runs;
            std::vector<GlyphRun> remainder_runs;
            auto runs = atom_it->getRuns();
            int last_whitespace_run_index = -1;
            int last_whitespace_glyph_index = -1;
            int total_glyphs_fitted = 0;

            bool all_runs_fit = true;
            for (int run_index = 0; run_index < runs.size(); run_index++) {
                const auto& run = runs[run_index];
                std::vector<GlyphMetrics> fitted_glyphs_in_run;
                std::vector<GlyphMetrics> remainder_glyphs_in_run;

                int glyph_index = 0;
                while (glyph_index < run.glyphs.size()) {
                    float glyph_width = run.glyphs[glyph_index].advance;
                    if (width_it + glyph_width > max_width) {
                        all_runs_fit = false;
                        break;
                    }
                    if (isWhitespace(run.glyphs[glyph_index].codepoint)) {
                        last_whitespace_run_index = run_index;
                        last_whitespace_glyph_index = fitted_glyphs_in_run.size();
                    }
                    fitted_glyphs_in_run.push_back(run.glyphs[glyph_index]);
                    width_it += glyph_width;
                    glyph_index++;
                    total_glyphs_fitted++;
                }

                if (!all_runs_fit) {
                    if (total_glyphs_fitted == 0 && wrapped_line.empty() && glyph_index < run.glyphs.size()) {
                        fitted_glyphs_in_run.push_back(run.glyphs[glyph_index]);
                        glyph_index++;
                        total_glyphs_fitted++;
                    }

                    while (glyph_index < run.glyphs.size()) {
                        remainder_glyphs_in_run.push_back(run.glyphs[glyph_index]);
                        glyph_index++;
                    }

                    if (!fitted_glyphs_in_run.empty()) {
                        fitted_runs.push_back({fitted_glyphs_in_run, run.font});
                    }
                    if (!remainder_glyphs_in_run.empty()) {
                        remainder_runs.push_back({remainder_glyphs_in_run, run.font});
                    }

                    for (int i = run_index + 1; i < runs.size(); i++) {
                        remainder_runs.push_back(runs[i]);
                    }
                    break;
                }

                if (!fitted_glyphs_in_run.empty()) {
                    fitted_runs.push_back({fitted_glyphs_in_run, run.font});
                }
            }

            if (mode == WrapMode::WORD && last_whitespace_run_index >= 0) {
                std::vector<GlyphRun> new_fitted_runs;
                std::vector<GlyphRun> new_remainder_runs;

                for (int i = 0; i < fitted_runs.size(); i++) {
                    if (i < last_whitespace_run_index) {
                        new_fitted_runs.push_back(fitted_runs[i]);
                    } else if (i == last_whitespace_run_index) {
                        std::vector<GlyphMetrics> fitted_part(
                            fitted_runs[i].glyphs.begin(),
                            fitted_runs[i].glyphs.begin() + last_whitespace_glyph_index
                        );
                        std::vector<GlyphMetrics> remainder_part(
                            fitted_runs[i].glyphs.begin() + last_whitespace_glyph_index + 1,
                            fitted_runs[i].glyphs.end()
                        );

                        if (!fitted_part.empty()) {
                            new_fitted_runs.push_back({fitted_part, fitted_runs[i].font});
                        }
                        if (!remainder_part.empty()) {
                            new_remainder_runs.push_back({remainder_part, fitted_runs[i].font});
                        }
                    } else {
                        new_remainder_runs.push_back(fitted_runs[i]);
                    }
                }

                new_remainder_runs.insert(new_remainder_runs.end(), remainder_runs.begin(), remainder_runs.end());

                std::vector<GlyphRun> merged_remainder_runs;
                for (const auto& run : new_remainder_runs) {
                    if (!merged_remainder_runs.empty() && merged_remainder_runs.back().font == run.font) {
                        merged_remainder_runs.back().glyphs.insert(
                            merged_remainder_runs.back().glyphs.end(),
                            run.glyphs.begin(),
                            run.glyphs.end()
                        );
                    } else {
                        merged_remainder_runs.push_back(run);
                    }
                }

                fitted_runs = new_fitted_runs;
                remainder_runs = merged_remainder_runs;
            }

            if (!fitted_runs.empty()) {
                wrapped_line.push_back(GlyphAtom(fitted_runs, atom_it->getFace()));
            }

            std::vector<GlyphAtom> remainder_atoms;

            if (!remainder_runs.empty()) {
                remainder_atoms.push_back(GlyphAtom(remainder_runs, atom_it->getFace()));
            }

            atom_it++;
            while(atom_it != atoms.end()) {
                remainder_atoms.push_back(*atom_it);
                atom_it++;
            }

            if (!remainder_atoms.empty()) {
                remainder = GlyphLine(remainder_atoms);
            } else {
                remainder = std::nullopt;
                line_index++;
            }
        }else {
            remainder = std::nullopt;
            line_index++;
        }

        wrapped_lines.push_back(GlyphLine(wrapped_line));
    }

    m_lines = wrapped_lines;
}

}
