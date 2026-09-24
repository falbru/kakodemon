#include "catch2/catch_test_macros.hpp"
#include "domain/atom.hpp"
#include "domain/codepointstring.hpp"
#include "domain/color.hpp"
#include "domain/face.hpp"
#include "domain/facespan.hpp"
#include "domain/geometry.hpp"
#include "domain/glyphlines.hpp"
#include "domain/glyphresolver.hpp"
#include "domain/line.hpp"
#include "domain/lines.hpp"
#include "domain/ports/font.hpp"
#include "domain/renderline.hpp"
#include "domain/renderlines.hpp"
#include "mock_glyphresolver.hpp"

TEST_CASE("RenderLines constructor with RenderLine vector argument", "[RenderLines]")
{
    SECTION("Empty lines")
    {
        domain::RenderLines render_lines(std::vector<domain::RenderLine>{}, 10.0f);
        REQUIRE(render_lines.getLines().size() == 0);
        REQUIRE(render_lines.getLineHeight() == 10.0f);
    }

    SECTION("Multiple lines")
    {
        domain::GlyphMetrics glyph = {
            .codepoint = 'a', .size = domain::UIVec2{20, 20}, .bearing = domain::IVec2{0, 0}, .advance = 20};

        domain::Face face1(domain::FixedColor::White, domain::FixedColor::Black, {});

        std::vector<domain::RenderLine> lines = {
            domain::RenderLine({glyph, glyph}, {domain::FaceSpan(face1, 0)}),
            domain::RenderLine({glyph, glyph, glyph}, {domain::FaceSpan(face1, 0), domain::FaceSpan(face1, 1)}),
        };

        domain::RenderLines render_lines(lines, 20.0f);
        REQUIRE(render_lines.getLineHeight() == 20.0f);
        REQUIRE(render_lines.getLines().size() == 2);
        REQUIRE(render_lines.getLines()[0].size() == 2);
        REQUIRE(render_lines.getLines()[0].getFaceSpans().size() == 1);
        REQUIRE(render_lines.getLines()[1].size() == 3);
        REQUIRE(render_lines.getLines()[1].getFaceSpans().size() == 2);
    }
}

TEST_CASE("RenderLines constructor with Lines argument", "[RenderLines]")
{
    GlyphResolverMock glyph_resolver(20);

    SECTION("Empty")
    {
        domain::RenderLines render_lines(domain::Lines(std::vector<domain::Line>{}), glyph_resolver, 10.0f);
        REQUIRE(render_lines.getLines().size() == 0);
        REQUIRE(render_lines.getLineHeight() == 10.0f);
    }

    SECTION("Multiple lines")
    {
        domain::Face face1(domain::FixedColor::White, domain::FixedColor::Black, {});

        domain::Line line1({domain::Atom(domain::CodepointString("HELLO"), face1)});
        domain::Line line2({domain::Atom(domain::CodepointString("WORLD"), face1)});

        domain::RenderLines render_lines(domain::Lines(std::vector<domain::Line>{line1, line2}), glyph_resolver, 25.0f);
        REQUIRE(render_lines.getLines().size() == 2);
        REQUIRE(render_lines.getLines()[0].size() == 5);
        REQUIRE(render_lines.getLines()[0].getGlyphs()[0].codepoint == 'H');
        REQUIRE(render_lines.getLines()[1].size() == 5);
        REQUIRE(render_lines.getLines()[1].getGlyphs()[0].codepoint == 'W');
        REQUIRE(render_lines.getLineHeight() == 25.0f);
    }
}

TEST_CASE("RenderLines size", "[RenderLines]")
{
    GlyphResolverMock glyph_resolver(10);

    SECTION("Empty")
    {
        domain::RenderLines render_lines(domain::Lines(std::vector<domain::Line>{}), glyph_resolver, 10.0f);
        REQUIRE(render_lines.size() == 0);
    }

    SECTION("Multiple lines")
    {
        domain::Face face1(domain::FixedColor::White, domain::FixedColor::Black, {});

        domain::Line line1({domain::Atom(domain::CodepointString("HELLO"), face1)});
        domain::Line line2({domain::Atom(domain::CodepointString("WORLD"), face1)});

        domain::RenderLines render_lines(domain::Lines(std::vector<domain::Line>{line1, line2}), glyph_resolver, 10.0f);
        REQUIRE(render_lines.size() == 2);
    }
}

TEST_CASE("RenderLines width", "[RenderLines]")
{
    GlyphResolverMock glyph_resolver(10);
    domain::Face face1(domain::FixedColor::White, domain::FixedColor::Black, {});

    SECTION("Empty")
    {
        domain::RenderLines render_lines(domain::Lines(std::vector<domain::Line>{}), glyph_resolver, 10.0f);
        REQUIRE(render_lines.width() == 0);
    }

    SECTION("One line, zero width")
    {
        domain::RenderLines render_lines(
            domain::Lines(std::vector<domain::Line>{domain::Line(std::vector<domain::Atom>{})}), glyph_resolver, 10.0f);
        REQUIRE(render_lines.width() == 0);
    }

    SECTION("Multiple lines, zero width")
    {
        domain::RenderLines render_lines(
            domain::Lines(std::vector<domain::Line>{domain::Line(std::vector<domain::Atom>{}),
                                                    domain::Line(std::vector<domain::Atom>{}),
                                                    domain::Line(std::vector<domain::Atom>{})}),
            glyph_resolver, 10.0f);
        REQUIRE(render_lines.width() == 0.0f);
    }

    SECTION("One line")
    {

        domain::Line line1({domain::Atom(domain::CodepointString("HELLO"), face1)});
        domain::Line line2({domain::Atom(domain::CodepointString("A SENTENCE."), face1)});

        domain::RenderLines render_lines1(domain::Lines(std::vector<domain::Line>{line1}), glyph_resolver, 10.0f);

        REQUIRE(render_lines1.width() == 50.0f);

        domain::RenderLines render_lines2(domain::Lines(std::vector<domain::Line>{line2}), glyph_resolver, 10.0f);

        REQUIRE(render_lines2.width() == 110.0f);
    }

    SECTION("Multiple lines")
    {
        domain::Line line1({domain::Atom(domain::CodepointString("HELLO"), face1)});
        domain::Line line2({domain::Atom(domain::CodepointString("WORLD!"), face1)});
        domain::Line line3({domain::Atom(domain::CodepointString("BYE"), face1)});

        domain::RenderLines render_lines(domain::Lines(std::vector<domain::Line>{line1, line2, line3}), glyph_resolver,
                                         10.0f);
        REQUIRE(render_lines.width() == 60.0f);
    }
}

TEST_CASE("RenderLines height", "[RenderLines]")
{
    GlyphResolverMock glyph_resolver(10);
    domain::Face face1(domain::FixedColor::White, domain::FixedColor::Black, {});

    SECTION("Empty")
    {
        domain::RenderLines render_lines(domain::Lines(std::vector<domain::Line>{}), glyph_resolver, 10.0f);
        REQUIRE(render_lines.height() == 0);
    }

    SECTION("One line, zero width")
    {
        domain::RenderLines render_lines(
            domain::Lines(std::vector<domain::Line>{domain::Line(std::vector<domain::Atom>{})}), glyph_resolver, 10.0f);
        REQUIRE(render_lines.height() == 10.0f);
    }

    SECTION("Multiple lines, zero width")
    {
        domain::RenderLines render_lines(
            domain::Lines(std::vector<domain::Line>{domain::Line(std::vector<domain::Atom>{}),
                                                    domain::Line(std::vector<domain::Atom>{}),
                                                    domain::Line(std::vector<domain::Atom>{})}),
            glyph_resolver, 10.0f);
        REQUIRE(render_lines.height() == 30.0f);
    }

    SECTION("One line")
    {

        domain::Line line1({domain::Atom(domain::CodepointString("HELLO"), face1)});

        domain::RenderLines render_lines1(domain::Lines(std::vector<domain::Line>{line1}), glyph_resolver, 10.0f);

        REQUIRE(render_lines1.height() == 10.0f);
    }

    SECTION("Multiple lines")
    {
        domain::Line line1({domain::Atom(domain::CodepointString("HELLO"), face1)});
        domain::Line line2({domain::Atom(domain::CodepointString("WORLD!"), face1)});
        domain::Line line3({domain::Atom(domain::CodepointString("BYE"), face1)});

        domain::RenderLines render_lines(domain::Lines(std::vector<domain::Line>{line1, line2, line3}), glyph_resolver,
                                         10.0f);
        REQUIRE(render_lines.height() == 30.0f);
    }
}

TEST_CASE("RenderLines wrap (WrapMode::CHARACTER)", "[RenderLines]")
{
    GlyphResolverMock glyph_resolver(10);
    domain::Face face1(domain::FixedColor::White, domain::FixedColor::Black, {});
    domain::Line line1({domain::Atom(domain::CodepointString("HELLO"), face1)});
    domain::Line line2({domain::Atom(domain::CodepointString("WORLD!"), face1)});
    domain::Line line3({domain::Atom(domain::CodepointString("BYE"), face1)});

    domain::Lines lines(std::vector<domain::Line>{line1, line2, line3});

    SECTION("Empty")
    {
        domain::RenderLines render_lines(domain::Lines(std::vector<domain::Line>{}), glyph_resolver, 10.0f);

        REQUIRE_NOTHROW(render_lines.wrap(50.0f, domain::RenderLinesWrapMode::CHARACTER));

        REQUIRE(render_lines.size() == 0);
    }

    SECTION("width() < max_width")
    {

        domain::RenderLines render_lines(lines, glyph_resolver, 10.0f);

        REQUIRE(render_lines.size() == 3);

        REQUIRE_NOTHROW(render_lines.wrap(100.0f, domain::RenderLinesWrapMode::CHARACTER));

        REQUIRE(render_lines.size() == 3);
        REQUIRE(render_lines.width() <= 100.0f);
    }

    SECTION("width() == max_width")
    {

        domain::RenderLines render_lines(lines, glyph_resolver, 10.0f);

        REQUIRE(render_lines.size() == 3);

        REQUIRE_NOTHROW(render_lines.wrap(60.0f, domain::RenderLinesWrapMode::CHARACTER));

        REQUIRE(render_lines.size() == 3);
        REQUIRE(render_lines.width() == 60.0f);
    }

    SECTION("max_width too small to fit a glyph")
    {
        domain::RenderLines render_lines(lines, glyph_resolver, 10.0f);

        REQUIRE_THROWS(render_lines.wrap(5.0f, domain::RenderLinesWrapMode::CHARACTER));
    }

    SECTION("Wrap a single line into two lines")
    {
        domain::RenderLines render_lines(domain::Lines({line1}), glyph_resolver, 10.0f);

        REQUIRE_NOTHROW(render_lines.wrap(33.0f, domain::RenderLinesWrapMode::CHARACTER));
        REQUIRE(render_lines.size() == 2);
        REQUIRE(render_lines.getLines()[0].size() == 3);
        REQUIRE(render_lines.getLines()[1].size() == 2);
        REQUIRE(render_lines.width() <= 33.0f);
    }

    SECTION("Wrap multiple lines")
    {
        domain::RenderLines render_lines(lines, glyph_resolver, 10.0f);

        REQUIRE_NOTHROW(render_lines.wrap(33.0f, domain::RenderLinesWrapMode::CHARACTER));
        REQUIRE(render_lines.size() == 5);
        REQUIRE(render_lines.getLines()[0].size() == 3);
        REQUIRE(render_lines.getLines()[1].size() == 2);
        REQUIRE(render_lines.getLines()[2].size() == 3);
        REQUIRE(render_lines.getLines()[3].size() == 3);
        REQUIRE(render_lines.getLines()[4].size() == 3);
        REQUIRE(render_lines.width() <= 33.0f);
    }

    SECTION("Lines must be wrapped multiple times")
    {
        domain::RenderLines render_lines(lines, glyph_resolver, 10.0f);

        REQUIRE_NOTHROW(render_lines.wrap(13.0f, domain::RenderLinesWrapMode::CHARACTER));
        REQUIRE(render_lines.size() == 14);

        for (int i = 0; i < 14; i++)
        {
            REQUIRE(render_lines.getLines()[0].size() == 1);
        }

        REQUIRE(render_lines.width() <= 13.0f);
    }
}

TEST_CASE("RenderLines wrap (WrapMode::WORD)", "[RenderLines]")
{
    GlyphResolverMock glyph_resolver(10);
    domain::Face face1(domain::FixedColor::White, domain::FixedColor::Black, {});
    domain::Line line1({domain::Atom(domain::CodepointString("GOODBYE MEANS THAT"), face1)});
    domain::Line line2({domain::Atom(domain::CodepointString("YOU'RE LOSING ME"), face1)});
    domain::Line line3({domain::Atom(domain::CodepointString("FOR LIFE"), face1)});

    domain::Lines lines(std::vector<domain::Line>{line1, line2, line3});

    SECTION("Empty")
    {
        domain::RenderLines render_lines(domain::Lines(std::vector<domain::Line>{}), glyph_resolver, 10.0f);

        REQUIRE_NOTHROW(render_lines.wrap(50.0f, domain::RenderLinesWrapMode::WORD));

        REQUIRE(render_lines.size() == 0);
    }

    SECTION("width() < max_width")
    {

        domain::RenderLines render_lines(lines, glyph_resolver, 10.0f);

        REQUIRE(render_lines.size() == 3);

        REQUIRE_NOTHROW(render_lines.wrap(500.0f, domain::RenderLinesWrapMode::WORD));

        REQUIRE(render_lines.size() == 3);
        REQUIRE(render_lines.width() <= 500.0f);
    }

    SECTION("width() == max_width")
    {

        domain::RenderLines render_lines(lines, glyph_resolver, 10.0f);

        REQUIRE(render_lines.size() == 3);

        REQUIRE_NOTHROW(render_lines.wrap(180.0f, domain::RenderLinesWrapMode::WORD));

        REQUIRE(render_lines.size() == 3);
        REQUIRE(render_lines.width() == 180.0f);
    }

    SECTION("max_width too small to fit a glyph")
    {
        domain::RenderLines render_lines(lines, glyph_resolver, 10.0f);

        REQUIRE_THROWS(render_lines.wrap(5.0f, domain::RenderLinesWrapMode::WORD));
    }

    SECTION("Wrap a single line into two lines")
    {
        domain::RenderLines render_lines(domain::Lines({line1}), glyph_resolver, 10.0f);

        REQUIRE_NOTHROW(render_lines.wrap(160.0f, domain::RenderLinesWrapMode::WORD));
        REQUIRE(render_lines.size() == 2);
        REQUIRE(render_lines.getLines()[0].size() == 13);
        REQUIRE(render_lines.getLines()[1].size() == 4);
        REQUIRE(render_lines.width() <= 160.0f);
    }

    SECTION("Wrap multiple lines")
    {
        domain::RenderLines render_lines(lines, glyph_resolver, 10.0f);

        REQUIRE_NOTHROW(render_lines.wrap(100.0f, domain::RenderLinesWrapMode::WORD));
        REQUIRE(render_lines.size() == 5);
        REQUIRE(render_lines.getLines()[0].size() == 7);
        REQUIRE(render_lines.getLines()[1].size() == 10);
        REQUIRE(render_lines.getLines()[2].size() == 6);
        REQUIRE(render_lines.getLines()[3].size() == 9);
        REQUIRE(render_lines.getLines()[4].size() == 8);
        REQUIRE(render_lines.width() <= 100.0f);
    }

    SECTION("Word is so long that it must wrap by character when using word wrap mode")
    {
        domain::RenderLines render_lines(lines, glyph_resolver, 10.0f);

        REQUIRE_NOTHROW(render_lines.wrap(50.0f, domain::RenderLinesWrapMode::WORD));
        REQUIRE(render_lines.size() == 10);
        REQUIRE(render_lines.getLines()[0].size() == 5);
        REQUIRE(render_lines.getLines()[1].size() == 2);
        REQUIRE(render_lines.getLines()[2].size() == 5);
        REQUIRE(render_lines.getLines()[3].size() == 4);
        REQUIRE(render_lines.getLines()[4].size() == 5);
        REQUIRE(render_lines.getLines()[5].size() == 1);
        REQUIRE(render_lines.getLines()[6].size() == 5);
        REQUIRE(render_lines.getLines()[7].size() == 4);
        REQUIRE(render_lines.getLines()[8].size() == 3);
        REQUIRE(render_lines.getLines()[9].size() == 4);

        REQUIRE(render_lines.width() <= 50.0f);
    }
}
