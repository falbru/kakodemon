#include <catch2/catch_test_macros.hpp>

#include "domain/color.hpp"
#include "domain/lines.hpp"
#include "domain/utf8string.hpp"
#include "mock_font.hpp"

TEST_CASE("GlyphLines handles empty line during wrap", "[GlyphLines][wrap][character]") {
    FontMock font;

    domain::Lines lines = domain::Lines({
        domain::Line(),
    });

    domain::GlyphLines glyph_lines = domain::GlyphLines(lines, &font);

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

    domain::GlyphLines glyph_lines = domain::GlyphLines(lines, &font);

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

    domain::GlyphLines glyph_lines = domain::GlyphLines(lines, &font);

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

    domain::GlyphLines glyph_lines = domain::GlyphLines(lines, &font);

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

    domain::GlyphLines glyph_lines = domain::GlyphLines(lines, &font);

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

    domain::GlyphLines glyph_lines = domain::GlyphLines(lines, &font);

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

    domain::GlyphLines glyph_lines = domain::GlyphLines(lines, &font);

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

    domain::GlyphLines glyph_lines = domain::GlyphLines(lines, &font);

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

    domain::GlyphLines glyph_lines = domain::GlyphLines(lines, &font);

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

    domain::GlyphLines glyph_lines = domain::GlyphLines(lines, &font);

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

    domain::GlyphLines glyph_lines = domain::GlyphLines(lines, &font);

    glyph_lines.wrap(100.0f, domain::WrapMode::WORD);

    auto wrapped_lines = glyph_lines.toLines();

    REQUIRE(wrapped_lines.size() == 1);
    REQUIRE(wrapped_lines.at(0).toUTF8String() == domain::UTF8String("123456 "));
}
