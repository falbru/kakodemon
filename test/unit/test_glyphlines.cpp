#include <catch2/catch_test_macros.hpp>

#include "domain/color.hpp"
#include "domain/glyphlinesbuilder.hpp"
#include "domain/lines.hpp"
#include "domain/utf8string.hpp"
#include "mock_font.hpp"

TEST_CASE("GlyphLines handles empty line during wrap", "[GlyphLines][wrap][character]") {
    FontMock font;

    domain::Lines lines = domain::Lines({
        domain::Line(),
    });

    domain::GlyphLines glyph_lines = domain::GlyphLinesBuilder::build(lines, &font);

    REQUIRE(glyph_lines.size() == 1);

    glyph_lines.wrap(100.0f, domain::WrapMode::CHARACTER);

    REQUIRE(glyph_lines.size() == 1);

    auto wrapped_lines = glyph_lines.toLines();

    REQUIRE(wrapped_lines.at(0).length() == 0);
}

TEST_CASE("GlyphLines preserves line that fits within max width", "[GlyphLines][wrap][character]") {
    FontMock font;

    domain::Lines lines = domain::Lines({
        domain::Line({domain::Atom(domain::UTF8String("12345"), domain::Face(domain::DefaultColor(), domain::DefaultColor()))})
    });

    domain::GlyphLines glyph_lines = domain::GlyphLinesBuilder::build(lines, &font);

    REQUIRE(glyph_lines.size() == 1);

    glyph_lines.wrap(100.0f, domain::WrapMode::CHARACTER);

    REQUIRE(glyph_lines.size() == 1);

    auto wrapped_lines = glyph_lines.toLines();

    REQUIRE(wrapped_lines.at(0).toUTF8String() == domain::UTF8String("12345"));
}

TEST_CASE("GlyphLines wraps exactly at max width boundary", "[GlyphLines][wrap][character]") {
    FontMock font;

    domain::Lines lines = domain::Lines({
        domain::Line({domain::Atom(domain::UTF8String("1234567890ABCDE"), domain::Face(domain::DefaultColor(), domain::DefaultColor()))})
    });

    domain::GlyphLines glyph_lines = domain::GlyphLinesBuilder::build(lines, &font);

    glyph_lines.wrap(100.0f, domain::WrapMode::CHARACTER);

    auto wrapped_lines = glyph_lines.toLines();

    REQUIRE(wrapped_lines.at(0).length() == 10);
    REQUIRE(wrapped_lines.at(1).length() == 5);
}

TEST_CASE("GlyphLines wraps single atom exceeding max width into multiple lines", "[GlyphLines][wrap][character]") {
    FontMock font;

    domain::Lines lines = domain::Lines({
        domain::Line({domain::Atom(domain::UTF8String("1234567890123456789012345"), domain::Face(domain::DefaultColor(), domain::DefaultColor()))})
    });

    domain::GlyphLines glyph_lines = domain::GlyphLinesBuilder::build(lines, &font);

    REQUIRE(glyph_lines.size() == 1);

    glyph_lines.wrap(100.0f, domain::WrapMode::CHARACTER);

    REQUIRE(glyph_lines.size() == 3);

    auto wrapped_lines = glyph_lines.toLines();

    REQUIRE(wrapped_lines.at(0).length() == 10);
    REQUIRE(wrapped_lines.at(1).length() == 10);
    REQUIRE(wrapped_lines.at(2).length() == 5);

    REQUIRE(wrapped_lines.at(0).toUTF8String() == domain::UTF8String("1234567890"));
    REQUIRE(wrapped_lines.at(1).toUTF8String() == domain::UTF8String("1234567890"));
    REQUIRE(wrapped_lines.at(2).toUTF8String() == domain::UTF8String("12345"));
}

TEST_CASE("GlyphLines wrapped lines are inserted before preceding lines", "[GlyphLines][wrap][character]") {
    FontMock font;

    domain::Lines lines = domain::Lines({
        domain::Line({domain::Atom(domain::UTF8String("123456789012345"), domain::Face(domain::DefaultColor(), domain::DefaultColor()))}),
        domain::Line({domain::Atom(domain::UTF8String("ABCDE"), domain::Face(domain::DefaultColor(), domain::DefaultColor()))}),
    });

    domain::GlyphLines glyph_lines = domain::GlyphLinesBuilder::build(lines, &font);

    REQUIRE(glyph_lines.size() == 2);

    glyph_lines.wrap(100.0f, domain::WrapMode::CHARACTER);

    REQUIRE(glyph_lines.size() == 3);

    auto wrapped_lines = glyph_lines.toLines();

    REQUIRE(wrapped_lines.at(0).length() == 10);
    REQUIRE(wrapped_lines.at(1).length() == 5);
    REQUIRE(wrapped_lines.at(2).length() == 5);

    REQUIRE(wrapped_lines.at(0).toUTF8String() == domain::UTF8String("1234567890"));
    REQUIRE(wrapped_lines.at(1).toUTF8String() == domain::UTF8String("12345"));
    REQUIRE(wrapped_lines.at(2).toUTF8String() == domain::UTF8String("ABCDE"));
}

TEST_CASE("GlyphLines wraps multiple atoms across line boundary", "[GlyphLines][wrap][character]") {
    FontMock font;

    domain::Lines lines = domain::Lines({
        domain::Line({
            domain::Atom(domain::UTF8String("12345"), domain::Face(domain::DefaultColor(), domain::DefaultColor())),
            domain::Atom(domain::UTF8String("67890"), domain::Face(domain::DefaultColor(), domain::DefaultColor())),
            domain::Atom(domain::UTF8String("ABCDE"), domain::Face(domain::DefaultColor(), domain::DefaultColor())),
            domain::Atom(domain::UTF8String("FGHIJ"), domain::Face(domain::DefaultColor(), domain::DefaultColor()))
        })
    });

    domain::GlyphLines glyph_lines = domain::GlyphLinesBuilder::build(lines, &font);

    REQUIRE(glyph_lines.size() == 1);

    glyph_lines.wrap(100.0f, domain::WrapMode::CHARACTER);

    REQUIRE(glyph_lines.size() == 2);

    auto wrapped_lines = glyph_lines.toLines();

    REQUIRE(wrapped_lines.at(0).length() == 10);
    REQUIRE(wrapped_lines.at(1).length() == 10);

    REQUIRE(wrapped_lines.at(0).size() == 2);
    REQUIRE(wrapped_lines.at(1).size() == 2);

    REQUIRE(wrapped_lines.at(0).toUTF8String() == domain::UTF8String("1234567890"));
    REQUIRE(wrapped_lines.at(1).toUTF8String() == domain::UTF8String("ABCDEFGHIJ"));
}

TEST_CASE("GlyphLines wraps at word boundaries when word wrap enabled", "[GlyphLines][wrap][word]") {
    FontMock font;

    domain::Lines lines = domain::Lines({
        domain::Line({
            domain::Atom(domain::UTF8String("a long sentence"), domain::Face(domain::DefaultColor(), domain::DefaultColor())),
        })
    });

    domain::GlyphLines glyph_lines = domain::GlyphLinesBuilder::build(lines, &font);

    REQUIRE(glyph_lines.size() == 1);

    glyph_lines.wrap(100.0f, domain::WrapMode::WORD);

    REQUIRE(glyph_lines.size() == 2);

    auto wrapped_lines = glyph_lines.toLines();

    REQUIRE(wrapped_lines.at(0).length() == 6);
    REQUIRE(wrapped_lines.at(1).length() == 8);

    REQUIRE(wrapped_lines.at(0).toUTF8String() == domain::UTF8String("a long"));
    REQUIRE(wrapped_lines.at(1).toUTF8String() == domain::UTF8String("sentence"));
}

TEST_CASE("GlyphLines word wrap falls back to character wrap when word exceeds max width", "[GlyphLines][wrap][word]") {
    FontMock font;

    domain::Lines lines = domain::Lines({
        domain::Line({
            domain::Atom(domain::UTF8String("short 1234567890ABCDEFG rest"), domain::Face(domain::DefaultColor(), domain::DefaultColor())),
        })
    });

    domain::GlyphLines glyph_lines = domain::GlyphLinesBuilder::build(lines, &font);

    glyph_lines.wrap(100.0f, domain::WrapMode::WORD);

    auto wrapped_lines = glyph_lines.toLines();

    REQUIRE(wrapped_lines.size() == 4);

    REQUIRE(wrapped_lines.at(0).length() == 5);
    REQUIRE(wrapped_lines.at(1).length() == 10);
    REQUIRE(wrapped_lines.at(2).length() == 7);
    REQUIRE(wrapped_lines.at(3).length() == 4);

    REQUIRE(wrapped_lines.at(0).toUTF8String() == domain::UTF8String("short"));
    REQUIRE(wrapped_lines.at(1).toUTF8String() == domain::UTF8String("1234567890"));
    REQUIRE(wrapped_lines.at(2).toUTF8String() == domain::UTF8String("ABCDEFG"));
    REQUIRE(wrapped_lines.at(3).toUTF8String() == domain::UTF8String("rest"));
}

TEST_CASE("GlyphLines word wrap handles multiple consecutive spaces", "[GlyphLines][wrap][word]") {
    FontMock font;

    domain::Lines lines = domain::Lines({
        domain::Line({
            domain::Atom(domain::UTF8String("123   567890AB"), domain::Face(domain::DefaultColor(), domain::DefaultColor())),
        })
    });

    domain::GlyphLines glyph_lines = domain::GlyphLinesBuilder::build(lines, &font);

    glyph_lines.wrap(100.0f, domain::WrapMode::WORD);

    auto wrapped_lines = glyph_lines.toLines();

    REQUIRE(wrapped_lines.size() == 2);
}

TEST_CASE("GlyphLines wraps multiple atoms with different faces", "[GlyphLines][wrap][character]") {
    FontMock font;

    domain::Face face1(domain::DefaultColor{}, domain::DefaultColor{});
    domain::Face face2(domain::RGBAColor{255, 0, 0, 255}, domain::DefaultColor());

    domain::Lines lines = domain::Lines({
        domain::Line({
            domain::Atom(domain::UTF8String("1234567890"), face1),
            domain::Atom(domain::UTF8String("ABCDEFGHIJ"), face2),
        })
    });

    domain::GlyphLines glyph_lines = domain::GlyphLinesBuilder::build(lines, &font);

    glyph_lines.wrap(100.0f, domain::WrapMode::CHARACTER);

    auto wrapped_lines = glyph_lines.toLines();

    REQUIRE(wrapped_lines.size() == 2);
    REQUIRE(wrapped_lines.at(0).size() == 1);
    REQUIRE(wrapped_lines.at(1).size() == 1);
}

TEST_CASE("GlyphLines wraps line ending with whitespace", "[GlyphLines][wrap][word]") {
    FontMock font;

    domain::Lines lines = domain::Lines({
        domain::Line({
            domain::Atom(domain::UTF8String("123456 "), domain::Face(domain::DefaultColor(), domain::DefaultColor())),
        })
    });

    domain::GlyphLines glyph_lines = domain::GlyphLinesBuilder::build(lines, &font);

    glyph_lines.wrap(100.0f, domain::WrapMode::WORD);

    auto wrapped_lines = glyph_lines.toLines();

    REQUIRE(wrapped_lines.size() == 1);
    REQUIRE(wrapped_lines.at(0).toUTF8String() == domain::UTF8String("123456 "));
}

TEST_CASE("GlyphLines handles max_width smaller than single glyph width", "[GlyphLines][wrap][character]") {
    FontMock font;

    domain::Lines lines = domain::Lines({
        domain::Line({
            domain::Atom(domain::UTF8String("ABC"), domain::Face(domain::DefaultColor(), domain::DefaultColor())),
        })
    });

    domain::GlyphLines glyph_lines = domain::GlyphLinesBuilder::build(lines, &font);

    glyph_lines.wrap(5.0f, domain::WrapMode::CHARACTER);

    auto wrapped_lines = glyph_lines.toLines();

    REQUIRE(wrapped_lines.size() == 3);
    REQUIRE(wrapped_lines.at(0).length() == 1);
    REQUIRE(wrapped_lines.at(1).length() == 1);
    REQUIRE(wrapped_lines.at(2).length() == 1);
    REQUIRE(wrapped_lines.at(0).toUTF8String() == domain::UTF8String("A"));
    REQUIRE(wrapped_lines.at(1).toUTF8String() == domain::UTF8String("B"));
    REQUIRE(wrapped_lines.at(2).toUTF8String() == domain::UTF8String("C"));
}

TEST_CASE("GlyphLines handles atom with multiple glyph runs", "[GlyphLines][wrap][glyphruns]") {
    FontMock font1;
    FontMock font2;

    std::vector<domain::GlyphMetrics> run1_glyphs;
    for (char c : {'A', 'B', 'C', 'D', 'E'}) {
        font1.loadGlyph(c);
        run1_glyphs.push_back(font1.getGlyphMetrics(c));
    }

    std::vector<domain::GlyphMetrics> run2_glyphs;
    for (char c : {'F', 'G', 'H', 'I', 'J'}) {
        font2.loadGlyph(c);
        run2_glyphs.push_back(font2.getGlyphMetrics(c));
    }

    std::vector<domain::GlyphRun> runs = {
        {run1_glyphs, &font1},
        {run2_glyphs, &font2}
    };

    domain::GlyphAtom glyph_atom(runs, domain::Face(domain::DefaultColor(), domain::DefaultColor()));

    REQUIRE(glyph_atom.getRuns().size() == 2);
    REQUIRE(glyph_atom.width() == 100.0f);

    domain::Atom atom = glyph_atom.toAtom();
    REQUIRE(atom.size() == 10);
}

TEST_CASE("GlyphLines wraps atom with single glyph run into multiple atoms", "[GlyphLines][wrap][glyphruns]") {
    FontMock font;

    domain::Lines lines = domain::Lines({
        domain::Line({
            domain::Atom(domain::UTF8String("1234567890ABCDE"), domain::Face(domain::DefaultColor(), domain::DefaultColor()))
        })
    });

    domain::GlyphLines glyph_lines = domain::GlyphLinesBuilder::build(lines, &font);

    auto initial_line = glyph_lines.getGlyphLines()[0];
    REQUIRE(initial_line.getGlyphAtoms().size() == 1);
    REQUIRE(initial_line.getGlyphAtoms()[0].getRuns().size() == 1);

    glyph_lines.wrap(100.0f, domain::WrapMode::CHARACTER);

    auto wrapped_lines = glyph_lines.toLines();

    REQUIRE(wrapped_lines.size() == 2);
    REQUIRE(wrapped_lines.at(0).length() == 10);
    REQUIRE(wrapped_lines.at(1).length() == 5);

    auto glyph_line0 = glyph_lines.getGlyphLines()[0];
    auto glyph_line1 = glyph_lines.getGlyphLines()[1];

    REQUIRE(glyph_line0.getGlyphAtoms().size() == 1);
    REQUIRE(glyph_line0.getGlyphAtoms()[0].getRuns().size() == 1);
    REQUIRE(glyph_line0.getGlyphAtoms()[0].getRuns()[0].glyphs.size() == 10);
    REQUIRE(glyph_line0.getGlyphAtoms()[0].getRuns()[0].font == &font);

    REQUIRE(glyph_line1.getGlyphAtoms().size() == 1);
    REQUIRE(glyph_line1.getGlyphAtoms()[0].getRuns().size() == 1);
    REQUIRE(glyph_line1.getGlyphAtoms()[0].getRuns()[0].glyphs.size() == 5);
    REQUIRE(glyph_line1.getGlyphAtoms()[0].getRuns()[0].font == &font);
}

TEST_CASE("GlyphLines wraps atom with multiple glyph runs", "[GlyphLines][wrap][glyphruns]") {
    FontMock font1;
    FontMock font2;

    std::vector<domain::GlyphMetrics> run1_glyphs;
    for (char c : {'1', '2', '3', '4', '5'}) {
        font1.loadGlyph(c);
        run1_glyphs.push_back(font1.getGlyphMetrics(c));
    }

    std::vector<domain::GlyphMetrics> run2_glyphs;
    for (char c : {'6', '7', '8', '9', '0', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'}) {
        font2.loadGlyph(c);
        run2_glyphs.push_back(font2.getGlyphMetrics(c));
    }

    std::vector<domain::GlyphRun> runs = {
        {run1_glyphs, &font1},
        {run2_glyphs, &font2}
    };

    domain::GlyphAtom glyph_atom(runs, domain::Face(domain::DefaultColor(), domain::DefaultColor()));

    domain::GlyphLine glyph_line({glyph_atom});
    std::vector<domain::GlyphLine> lines = {glyph_line};
    domain::GlyphLines glyph_lines(lines, 24.0f);

    REQUIRE(glyph_lines.size() == 1);
    REQUIRE(glyph_lines.getGlyphLines()[0].getGlyphAtoms().size() == 1);
    REQUIRE(glyph_lines.getGlyphLines()[0].getGlyphAtoms()[0].getRuns().size() == 2);

    glyph_lines.wrap(100.0f, domain::WrapMode::CHARACTER);

    auto wrapped_lines = glyph_lines.toLines();

    REQUIRE(wrapped_lines.size() == 2);
    REQUIRE(wrapped_lines.at(0).length() == 10);
    REQUIRE(wrapped_lines.at(1).length() == 8);

    auto glyph_line0 = glyph_lines.getGlyphLines()[0];
    auto glyph_line1 = glyph_lines.getGlyphLines()[1];

    REQUIRE(glyph_line0.getGlyphAtoms().size() == 1);
    REQUIRE(glyph_line0.getGlyphAtoms()[0].getRuns().size() == 2);
    REQUIRE(glyph_line0.getGlyphAtoms()[0].getRuns()[0].glyphs.size() == 5);
    REQUIRE(glyph_line0.getGlyphAtoms()[0].getRuns()[0].font == &font1);
    REQUIRE(glyph_line0.getGlyphAtoms()[0].getRuns()[1].glyphs.size() == 5);
    REQUIRE(glyph_line0.getGlyphAtoms()[0].getRuns()[1].font == &font2);

    REQUIRE(glyph_line1.getGlyphAtoms().size() == 1);
    REQUIRE(glyph_line1.getGlyphAtoms()[0].getRuns().size() == 1);
    REQUIRE(glyph_line1.getGlyphAtoms()[0].getRuns()[0].glyphs.size() == 8);
    REQUIRE(glyph_line1.getGlyphAtoms()[0].getRuns()[0].font == &font2);
}

TEST_CASE("GlyphLines wraps atom with multiple runs at run boundary", "[GlyphLines][wrap][glyphruns]") {
    FontMock font1;
    FontMock font2;

    std::vector<domain::GlyphMetrics> run1_glyphs;
    for (char c : {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1'}) {
        font1.loadGlyph(c);
        run1_glyphs.push_back(font1.getGlyphMetrics(c));
    }

    std::vector<domain::GlyphMetrics> run2_glyphs;
    for (char c : {'A', 'B', 'C', 'D', 'E'}) {
        font2.loadGlyph(c);
        run2_glyphs.push_back(font2.getGlyphMetrics(c));
    }

    std::vector<domain::GlyphRun> runs = {
        {run1_glyphs, &font1},
        {run2_glyphs, &font2}
    };

    domain::GlyphAtom glyph_atom(runs, domain::Face(domain::DefaultColor(), domain::DefaultColor()));
    domain::GlyphLine glyph_line({glyph_atom});
    std::vector<domain::GlyphLine> lines = {glyph_line};
    domain::GlyphLines glyph_lines(lines, 24.0f);

    glyph_lines.wrap(110.0f, domain::WrapMode::CHARACTER);

    auto wrapped_lines = glyph_lines.toLines();

    REQUIRE(wrapped_lines.size() == 2);
    REQUIRE(wrapped_lines.at(0).length() == 11);
    REQUIRE(wrapped_lines.at(1).length() == 5);

    auto glyph_line0 = glyph_lines.getGlyphLines()[0];
    auto glyph_line1 = glyph_lines.getGlyphLines()[1];

    REQUIRE(glyph_line0.getGlyphAtoms().size() == 1);
    REQUIRE(glyph_line0.getGlyphAtoms()[0].getRuns().size() == 1);
    REQUIRE(glyph_line0.getGlyphAtoms()[0].getRuns()[0].font == &font1);

    REQUIRE(glyph_line1.getGlyphAtoms().size() == 1);
    REQUIRE(glyph_line1.getGlyphAtoms()[0].getRuns().size() == 1);
    REQUIRE(glyph_line1.getGlyphAtoms()[0].getRuns()[0].font == &font2);
}

TEST_CASE("GlyphLines word wrap splits atom with multiple runs", "[GlyphLines][wrap][word][glyphruns]") {
    FontMock font1;
    FontMock font2;

    std::vector<domain::GlyphMetrics> run1_glyphs;
    for (char c : {'h', 'e', 'l', 'l', 'o', ' '}) {
        font1.loadGlyph(c);
        run1_glyphs.push_back(font1.getGlyphMetrics(c));
    }

    std::vector<domain::GlyphMetrics> run2_glyphs;
    for (char c : {'w', 'o', 'r', 'l', 'd', ' ', 't', 'e', 's', 't'}) {
        font2.loadGlyph(c);
        run2_glyphs.push_back(font2.getGlyphMetrics(c));
    }

    std::vector<domain::GlyphRun> runs = {
        {run1_glyphs, &font1},
        {run2_glyphs, &font2}
    };

    domain::GlyphAtom glyph_atom(runs, domain::Face(domain::DefaultColor(), domain::DefaultColor()));
    domain::GlyphLine glyph_line({glyph_atom});
    std::vector<domain::GlyphLine> lines = {glyph_line};
    domain::GlyphLines glyph_lines(lines, 24.0f);

    glyph_lines.wrap(100.0f, domain::WrapMode::WORD);

    auto wrapped_lines = glyph_lines.toLines();

    REQUIRE(wrapped_lines.size() == 2);
    REQUIRE(wrapped_lines.at(0).length() == 5);
    REQUIRE(wrapped_lines.at(1).length() == 10);

    auto glyph_line0 = glyph_lines.getGlyphLines()[0];
    auto glyph_line1 = glyph_lines.getGlyphLines()[1];

    REQUIRE(glyph_line0.getGlyphAtoms().size() == 1);
    REQUIRE(glyph_line0.getGlyphAtoms()[0].getRuns().size() == 1);
    REQUIRE(glyph_line0.getGlyphAtoms()[0].getRuns()[0].font == &font1);
    REQUIRE(glyph_line0.getGlyphAtoms()[0].getRuns()[0].glyphs.size() == 5);

    REQUIRE(glyph_line1.getGlyphAtoms().size() == 1);
    REQUIRE(glyph_line1.getGlyphAtoms()[0].getRuns().size() == 1);
    REQUIRE(glyph_line1.getGlyphAtoms()[0].getRuns()[0].font == &font2);
    REQUIRE(glyph_line1.getGlyphAtoms()[0].getRuns()[0].glyphs.size() == 10);
}
