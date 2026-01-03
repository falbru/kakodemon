#include "glyphlinesbuilder.hpp"
#include <algorithm>

namespace domain
{

GlyphLines GlyphLinesBuilder::build(const Lines& lines, Font* font, FontManager* font_manager)
{
    std::vector<GlyphLine> glyph_lines;
    glyph_lines.reserve(lines.size());

    for (int i = 0; i < lines.size(); i++) {
        glyph_lines.push_back(build(lines.getLines()[i], font, font_manager));
    }

    float line_height = font->getLineHeight();
    return GlyphLines(glyph_lines, line_height);
}

GlyphLine GlyphLinesBuilder::build(const Line& line, Font* font, FontManager* font_manager)
{
    std::vector<GlyphAtom> glyph_atoms;
    glyph_atoms.reserve(line.size());

    auto line_atoms = line.getAtoms();
    for (int i = 0; i < line.size(); i++) {
        glyph_atoms.push_back(build(line_atoms[i], font, font_manager));
    }

    return GlyphLine(glyph_atoms);
}

GlyphAtom GlyphLinesBuilder::build(const Atom& atom, Font* font, FontManager* font_manager)
{
    auto contents = atom.getContents();
    const auto& attributes = atom.getFace().getAttributes();

    bool has_bold = std::find(attributes.begin(), attributes.end(), Attribute::Bold) != attributes.end();
    bool has_italic = std::find(attributes.begin(), attributes.end(), Attribute::Italic) != attributes.end();

    Font* active_font = font;
    if (has_bold || has_italic) {
        FontStyle style;
        if (has_bold && has_italic) {
            style = FontStyle::BoldItalic;
        } else if (has_bold) {
            style = FontStyle::Bold;
        } else {
            style = FontStyle::Italic;
        }

        Font* variant_font = font_manager->getFontStyleVariant(font, style);
        if (variant_font != nullptr) {
            active_font = variant_font;
        }
    }

    auto runs = buildGlyphRuns(contents, active_font, font_manager);
    return GlyphAtom(runs, atom.getFace());
}

std::vector<GlyphRun> GlyphLinesBuilder::buildGlyphRuns(const UTF8String& contents, Font* font, FontManager* font_manager)
{
    std::vector<GlyphRun> runs;

    if (contents.size() == 0) {
        return runs;
    }

    std::vector<GlyphMetrics> current_glyphs;
    Font* current_font = nullptr;

    for (int i = 0; i < contents.size(); i++) {
        auto glyph_with_font = font_manager->getGlyphWithFont(contents.at(i), font);

        if (current_font == nullptr || current_font == glyph_with_font.font) {
            current_font = glyph_with_font.font;
            current_glyphs.push_back(glyph_with_font.glyph);
        } else {
            runs.push_back(GlyphRun{current_glyphs, current_font});
            current_glyphs.clear();
            current_font = glyph_with_font.font;
            current_glyphs.push_back(glyph_with_font.glyph);
        }
    }

    if (!current_glyphs.empty()) {
        runs.push_back(GlyphRun{current_glyphs, current_font});
    }

    return runs;
}

std::vector<GlyphRun> GlyphLinesBuilder::buildGlyphRuns(const UTF8String& contents, Font* font)
{
    std::vector<GlyphMetrics> glyphs;
    glyphs.reserve(contents.size());

    for (int i = 0; i < contents.size(); i++) {
        font->loadGlyph(contents.at(i));
        const GlyphMetrics& glyph = font->getGlyphMetrics(contents.at(i));
        glyphs.push_back(glyph);
    }

    std::vector<GlyphRun> runs;
    if (!glyphs.empty()) {
        runs.push_back(GlyphRun{glyphs, font});
    }
    return runs;
}

GlyphLines GlyphLinesBuilder::build(const Lines& lines, Font* font)
{
    std::vector<GlyphLine> glyph_lines;
    glyph_lines.reserve(lines.size());

    for (int i = 0; i < lines.size(); i++) {
        glyph_lines.push_back(build(lines.getLines()[i], font));
    }

    float line_height = font->getLineHeight();
    return GlyphLines(glyph_lines, line_height);
}

GlyphLine GlyphLinesBuilder::build(const Line& line, Font* font)
{
    std::vector<GlyphAtom> glyph_atoms;
    glyph_atoms.reserve(line.size());

    auto line_atoms = line.getAtoms();
    for (int i = 0; i < line.size(); i++) {
        glyph_atoms.push_back(build(line_atoms[i], font));
    }

    return GlyphLine(glyph_atoms);
}

GlyphAtom GlyphLinesBuilder::build(const Atom& atom, Font* font)
{
    auto contents = atom.getContents();
    auto runs = buildGlyphRuns(contents, font);
    return GlyphAtom(runs, atom.getFace());
}

} // namespace domain
