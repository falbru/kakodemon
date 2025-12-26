#include <catch2/catch_test_macros.hpp>

#include "domain/color.hpp"
#include "domain/glyphlinesbuilder.hpp"
#include "domain/lines.hpp"
#include "domain/utf8string.hpp"
#include "mock_font.hpp"

TEST_CASE("GlyphLinesBuilder builds GlyphAtom from Atom", "[GlyphLinesBuilder]") {
    FontMock font;

    domain::Atom atom(domain::UTF8String("hello"), domain::Face(domain::DefaultColor(), domain::DefaultColor()));

    domain::GlyphAtom glyph_atom = domain::GlyphLinesBuilder::build(atom, &font);

    REQUIRE(glyph_atom.getRuns().size() == 1);
    REQUIRE(glyph_atom.getRuns()[0].glyphs.size() == 5);
    REQUIRE(glyph_atom.getRuns()[0].font == &font);
    REQUIRE(glyph_atom.getFace() == atom.getFace());
    REQUIRE(glyph_atom.width() == 50.0f);
}

TEST_CASE("GlyphLinesBuilder builds GlyphLine from Line", "[GlyphLinesBuilder]") {
    FontMock font;

    domain::Line line({
        domain::Atom(domain::UTF8String("hello"), domain::Face(domain::DefaultColor(), domain::DefaultColor())),
        domain::Atom(domain::UTF8String(" "), domain::Face(domain::DefaultColor(), domain::DefaultColor())),
        domain::Atom(domain::UTF8String("world"), domain::Face(domain::DefaultColor(), domain::DefaultColor()))
    });

    domain::GlyphLine glyph_line = domain::GlyphLinesBuilder::build(line, &font);

    REQUIRE(glyph_line.getGlyphAtoms().size() == 3);
    REQUIRE(glyph_line.width() == 110.0f);
}

TEST_CASE("GlyphLinesBuilder builds GlyphLines from Lines", "[GlyphLinesBuilder]") {
    FontMock font;

    domain::Lines lines({
        domain::Line({domain::Atom(domain::UTF8String("line1"), domain::Face(domain::DefaultColor(), domain::DefaultColor()))}),
        domain::Line({domain::Atom(domain::UTF8String("line2"), domain::Face(domain::DefaultColor(), domain::DefaultColor()))})
    });

    domain::GlyphLines glyph_lines = domain::GlyphLinesBuilder::build(lines, &font);

    REQUIRE(glyph_lines.size() == 2);
    REQUIRE(glyph_lines.width() == 50.0f);
    REQUIRE(glyph_lines.height() == 48.0f);
}

TEST_CASE("GlyphLinesBuilder handles empty Atom", "[GlyphLinesBuilder]") {
    FontMock font;

    domain::Atom atom(domain::UTF8String(""), domain::Face(domain::DefaultColor(), domain::DefaultColor()));

    domain::GlyphAtom glyph_atom = domain::GlyphLinesBuilder::build(atom, &font);

    REQUIRE(glyph_atom.getRuns().size() == 0);
    REQUIRE(glyph_atom.width() == 0.0f);
}

TEST_CASE("GlyphLinesBuilder handles empty Line", "[GlyphLinesBuilder]") {
    FontMock font;

    domain::Line line;

    domain::GlyphLine glyph_line = domain::GlyphLinesBuilder::build(line, &font);

    REQUIRE(glyph_line.getGlyphAtoms().size() == 0);
    REQUIRE(glyph_line.width() == 0.0f);
}

TEST_CASE("GlyphLinesBuilder handles empty Lines", "[GlyphLinesBuilder]") {
    FontMock font;

    domain::Lines lines;

    domain::GlyphLines glyph_lines = domain::GlyphLinesBuilder::build(lines, &font);

    REQUIRE(glyph_lines.size() == 0);
    REQUIRE(glyph_lines.width() == 0.0f);
    REQUIRE(glyph_lines.height() == 0.0f);
}

TEST_CASE("GlyphLinesBuilder can convert back to domain types", "[GlyphLinesBuilder]") {
    FontMock font;

    domain::Lines original_lines({
        domain::Line({domain::Atom(domain::UTF8String("test"), domain::Face(domain::DefaultColor(), domain::DefaultColor()))})
    });

    domain::GlyphLines glyph_lines = domain::GlyphLinesBuilder::build(original_lines, &font);
    domain::Lines converted_lines = glyph_lines.toLines();

    REQUIRE(converted_lines.size() == original_lines.size());
    REQUIRE(converted_lines.at(0).toUTF8String() == original_lines.at(0).toUTF8String());
}
