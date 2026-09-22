#include <catch2/catch_test_macros.hpp>

#include "domain/color.hpp"
#include "domain/glyphatom.hpp"
#include "domain/glyphline.hpp"
#include "mock_font.hpp"
#include "mock_fontmanager.hpp"


TEST_CASE("GlyphLine truncate does not truncate when width equals max_width", "[GlyphLine][truncate]") {
    FontMock font;
    FontManagerMock font_manager;

    std::vector<domain::GlyphMetrics> glyphs;
    for (char c : {'A', 'B', 'C', 'D', 'E'}) {
        font.loadGlyph(c);
        glyphs.push_back(font.getGlyphMetrics(c));
    }

    std::vector<domain::GlyphRun> runs = {
        {glyphs, &font}
    };

    domain::GlyphAtom glyph_atom(runs, domain::Face(domain::DefaultColor(), domain::DefaultColor()));
    domain::GlyphLine glyph_line({glyph_atom});

    REQUIRE(glyph_line.width() == 50.0f);

    glyph_line.truncate(50.0f, &font, &font_manager);

    REQUIRE(glyph_line.getGlyphAtoms().size() == 1);
    REQUIRE(glyph_line.getGlyphAtoms()[0].getRuns().size() == 1);
    REQUIRE(glyph_line.getGlyphAtoms()[0].getRuns()[0].glyphs.size() == 5);
    REQUIRE(glyph_line.width() == 50.0f);
}

TEST_CASE("GlyphLine truncate does not truncate when width is less than max_width", "[GlyphLine][truncate]") {
    FontMock font;
    FontManagerMock font_manager;

    std::vector<domain::GlyphMetrics> glyphs;
    for (char c : {'A', 'B', 'C'}) {
        font.loadGlyph(c);
        glyphs.push_back(font.getGlyphMetrics(c));
    }

    std::vector<domain::GlyphRun> runs = {
        {glyphs, &font}
    };

    domain::GlyphAtom glyph_atom(runs, domain::Face(domain::DefaultColor(), domain::DefaultColor()));
    domain::GlyphLine glyph_line({glyph_atom});

    REQUIRE(glyph_line.width() == 30.0f);

    glyph_line.truncate(50.0f, &font, &font_manager);

    REQUIRE(glyph_line.getGlyphAtoms().size() == 1);
    REQUIRE(glyph_line.getGlyphAtoms()[0].getRuns()[0].glyphs.size() == 3);
    REQUIRE(glyph_line.width() == 30.0f);
}

TEST_CASE("GlyphLine truncate truncates when width exceeds max_width", "[GlyphLine][truncate]") {
    FontMock font;
    FontManagerMock font_manager;

    std::vector<domain::GlyphMetrics> glyphs;
    for (char c : {'A', 'B', 'C', 'D', 'E'}) {
        font.loadGlyph(c);
        glyphs.push_back(font.getGlyphMetrics(c));
    }

    std::vector<domain::GlyphRun> runs = {
        {glyphs, &font}
    };

    domain::GlyphAtom glyph_atom(runs, domain::Face(domain::DefaultColor(), domain::DefaultColor()));
    domain::GlyphLine glyph_line({glyph_atom});

    REQUIRE(glyph_line.width() == 50.0f);

    glyph_line.truncate(25.0f, &font, &font_manager);

    REQUIRE(glyph_line.getGlyphAtoms().size() == 1);
    REQUIRE(glyph_line.getGlyphAtoms()[0].getRuns().size() == 2);
    REQUIRE(glyph_line.getGlyphAtoms()[0].getRuns()[0].glyphs.size() == 1);
    REQUIRE(glyph_line.getGlyphAtoms()[0].getRuns()[1].glyphs.size() == 1);
}

TEST_CASE("GlyphLine truncate with single atom that needs truncation", "[GlyphLine][truncate]") {
    FontMock font;
    FontManagerMock font_manager;

    std::vector<domain::GlyphMetrics> glyphs;
    for (char c : {'A', 'B', 'C'}) {
        font.loadGlyph(c);
        glyphs.push_back(font.getGlyphMetrics(c));
    }

    std::vector<domain::GlyphRun> runs = {
        {glyphs, &font}
    };

    domain::GlyphAtom glyph_atom(runs, domain::Face(domain::DefaultColor(), domain::DefaultColor()));
    domain::GlyphLine glyph_line({glyph_atom});

    REQUIRE(glyph_line.width() == 30.0f);

    glyph_line.truncate(15.0f, &font, &font_manager);

    REQUIRE(glyph_line.width() <= 30.0f);
    REQUIRE(glyph_line.getGlyphAtoms().size() == 1);
    REQUIRE(glyph_line.getGlyphAtoms()[0].getRuns().size() == 1);
    REQUIRE(glyph_line.getGlyphAtoms()[0].getRuns()[0].glyphs.size() == 1);
}

TEST_CASE("GlyphLine truncate with multiple atoms that need truncation", "[GlyphLine][truncate]") {
    FontMock font;
    FontManagerMock font_manager;

    std::vector<domain::GlyphMetrics> glyphs1;
    for (char c : {'A', 'B'}) {
        font.loadGlyph(c);
        glyphs1.push_back(font.getGlyphMetrics(c));
    }

    std::vector<domain::GlyphMetrics> glyphs2;
    for (char c : {'C', 'D'}) {
        font.loadGlyph(c);
        glyphs2.push_back(font.getGlyphMetrics(c));
    }

    std::vector<domain::GlyphRun> runs1 = {
        {glyphs1, &font}
    };
    std::vector<domain::GlyphRun> runs2 = {
        {glyphs2, &font}
    };

    domain::GlyphAtom atom1(runs1, domain::Face(domain::DefaultColor(), domain::DefaultColor()));
    domain::GlyphAtom atom2(runs2, domain::Face(domain::DefaultColor(), domain::DefaultColor()));
    domain::GlyphLine glyph_line({atom1, atom2});

    REQUIRE(glyph_line.width() == 40.0f);

    glyph_line.truncate(35.0f, &font, &font_manager);

    REQUIRE(glyph_line.width() <= 35.0f);
    REQUIRE(glyph_line.getGlyphAtoms().size() == 1);
    REQUIRE(glyph_line.getGlyphAtoms()[0].getRuns().size() == 2);
    REQUIRE(glyph_line.getGlyphAtoms()[0].getRuns()[0].glyphs.size() == 2);
}

TEST_CASE("GlyphLine truncate with multiple atoms and multiple runs that need truncation", "[GlyphLine][truncate]") {
    FontMock font1;
    FontMock font2;
    FontManagerMock font_manager;

    std::vector<domain::GlyphMetrics> glyphs1a;
    for (char c : {'A', 'B'}) {
        font1.loadGlyph(c);
        glyphs1a.push_back(font1.getGlyphMetrics(c));
    }

    std::vector<domain::GlyphMetrics> glyphs1b;
    for (char c : {'C', 'D'}) {
        font2.loadGlyph(c);
        glyphs1b.push_back(font2.getGlyphMetrics(c));
    }

    std::vector<domain::GlyphRun> runs1 = {
        {glyphs1a, &font1},
        {glyphs1b, &font2}
    };

    std::vector<domain::GlyphMetrics> glyphs2;
    for (char c : {'E', 'F'}) {
        font1.loadGlyph(c);
        glyphs2.push_back(font1.getGlyphMetrics(c));
    }

    std::vector<domain::GlyphRun> runs2 = {
        {glyphs2, &font1}
    };

    domain::GlyphAtom atom1(runs1, domain::Face(domain::DefaultColor(), domain::DefaultColor()));
    domain::GlyphAtom atom2(runs2, domain::Face(domain::DefaultColor(), domain::DefaultColor()));
    domain::GlyphLine glyph_line({atom1, atom2});

    REQUIRE(glyph_line.width() == 60.0f);

    glyph_line.truncate(40.0f, &font1, &font_manager);

    REQUIRE(glyph_line.getGlyphAtoms().size() == 1);
    REQUIRE(glyph_line.getGlyphAtoms()[0].getRuns().size() == 3);
}

TEST_CASE("GlyphLine truncate removes all atoms when max_width is too small", "[GlyphLine][truncate]") {
    FontMock font;
    FontManagerMock font_manager;

    std::vector<domain::GlyphMetrics> glyphs;
    for (char c : {'A', 'B'}) {
        font.loadGlyph(c);
        glyphs.push_back(font.getGlyphMetrics(c));
    }

    std::vector<domain::GlyphRun> runs = {
        {glyphs, &font}
    };

    domain::GlyphAtom glyph_atom(runs, domain::Face(domain::DefaultColor(), domain::DefaultColor()));
    domain::GlyphLine glyph_line({glyph_atom});

    REQUIRE(glyph_line.width() == 20.0f);

    glyph_line.truncate(5.0f, &font, &font_manager);

    REQUIRE(glyph_line.width() == 0);
    REQUIRE(glyph_line.getGlyphAtoms().size() == 0);
}

TEST_CASE("GlyphLine truncate with empty line", "[GlyphLine][truncate]") {
    FontMock font;
    FontManagerMock font_manager;

    domain::GlyphLine glyph_line({});

    REQUIRE(glyph_line.width() == 0.0f);

    glyph_line.truncate(10.0f, &font, &font_manager);

    REQUIRE(glyph_line.getGlyphAtoms().size() == 0);
    REQUIRE(glyph_line.width() == 0.0f);
}
