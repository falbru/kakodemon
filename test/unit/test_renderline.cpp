#include <catch2/catch_test_macros.hpp>

#include "domain/atom.hpp"
#include "domain/codepointstring.hpp"
#include "domain/color.hpp"
#include "domain/face.hpp"
#include "domain/facespan.hpp"
#include "domain/geometry.hpp"
#include "domain/ports/font.hpp"
#include "domain/renderline.hpp"
#include "mock_glyphresolver.hpp"

TEST_CASE("RenderLine constructor with glyphs and face_spans", "[RenderLine]")
{
    std::vector<domain::GlyphMetrics> glyphs = {domain::GlyphMetrics{
                                                    'A',
                                                    domain::UIVec2{20, 30},
                                                    domain::IVec2{0, 0},
                                                    20,
                                                },
                                                domain::GlyphMetrics{
                                                    'B',
                                                    domain::UIVec2{20, 30},
                                                    domain::IVec2{0, 0},
                                                    20,
                                                },
                                                domain::GlyphMetrics{
                                                    'C',
                                                    domain::UIVec2{20, 30},
                                                    domain::IVec2{0, 0},
                                                    20,
                                                }};

    auto face1 = domain::Face(domain::FixedColor::White, domain::FixedColor::Black, {});

    SECTION("Handle sorted face spans")
    {
        std::vector<domain::FaceSpan> face_spans = {domain::FaceSpan(face1, 0)};
        domain::RenderLine render_line(glyphs, face_spans);

        REQUIRE(render_line.getGlyphs().size() == 3);
        REQUIRE(render_line.getGlyphs()[0].codepoint == 'A');
        REQUIRE(render_line.getGlyphs()[1].codepoint == 'B');
        REQUIRE(render_line.getGlyphs()[2].codepoint == 'C');
        REQUIRE(render_line.getFaceSpans().size() == 1);
        REQUIRE(render_line.getFaceSpans()[0].face == face1);
        REQUIRE(render_line.getFaceSpans()[0].start_index == 0);
    }

    SECTION("Handle unsorted face spans")
    {
        domain::RenderLine render_line(
            glyphs, {domain::FaceSpan(face1, 2), domain::FaceSpan(face1, 0), domain::FaceSpan(face1, 1)});

        REQUIRE(render_line.getFaceSpans().size() == 3);
        REQUIRE(render_line.getFaceSpans()[0].start_index == 0);
        REQUIRE(render_line.getFaceSpans()[1].start_index == 1);
        REQUIRE(render_line.getFaceSpans()[2].start_index == 2);
    }

    SECTION("Handle face span start_index that exceeds glyph size")
    {
        REQUIRE_THROWS(domain::RenderLine(glyphs, {domain::FaceSpan(face1, 10), domain::FaceSpan(face1, 0)}));
    }
}

TEST_CASE("RenderLine constructor with line and glyph resolver", "[RenderLine]")
{
    auto glyph_resolver = GlyphResolverMock(10);

    auto face1 = domain::Face(domain::FixedColor::White, domain::FixedColor::Black, {});
    auto face2 = domain::Face(domain::FixedColor::Red, domain::FixedColor::Blue, {});

    SECTION("Empty line")
    {
        domain::Line line;
        domain::RenderLine render_line(line, glyph_resolver);

        REQUIRE(render_line.getGlyphs().size() == 0);
        REQUIRE(render_line.getFaceSpans().size() == 0);
    }

    SECTION("Line with one atom")
    {
        domain::Line line({domain::Atom(domain::CodepointString("ABC"), face1)});
        domain::RenderLine render_line(line, glyph_resolver);

        REQUIRE(render_line.getGlyphs().size() == 3);
        REQUIRE(render_line.getGlyphs()[0].codepoint == 'A');
        REQUIRE(render_line.getGlyphs()[1].codepoint == 'B');
        REQUIRE(render_line.getGlyphs()[2].codepoint == 'C');
        REQUIRE(render_line.getFaceSpans().size() == 1);
        REQUIRE(render_line.getFaceSpans()[0].face == face1);
        REQUIRE(render_line.getFaceSpans()[0].start_index == 0);
    }

    SECTION("Line with multiple atoms")
    {
        domain::Line line(
            {domain::Atom(domain::CodepointString("ABC"), face1), domain::Atom(domain::CodepointString("DEF"), face2)});
        domain::RenderLine render_line(line, glyph_resolver);

        REQUIRE(render_line.getGlyphs().size() == 6);
        REQUIRE(render_line.getGlyphs()[0].codepoint == 'A');
        REQUIRE(render_line.getGlyphs()[1].codepoint == 'B');
        REQUIRE(render_line.getGlyphs()[2].codepoint == 'C');
        REQUIRE(render_line.getGlyphs()[3].codepoint == 'D');
        REQUIRE(render_line.getGlyphs()[4].codepoint == 'E');
        REQUIRE(render_line.getGlyphs()[5].codepoint == 'F');
        REQUIRE(render_line.getFaceSpans().size() == 2);
        REQUIRE(render_line.getFaceSpans()[0].face == face1);
        REQUIRE(render_line.getFaceSpans()[0].start_index == 0);
        REQUIRE(render_line.getFaceSpans()[1].face == face2);
        REQUIRE(render_line.getFaceSpans()[1].start_index == 3);
    }
}

TEST_CASE("RenderLine size", "[RenderLine]")
{
    std::vector<domain::GlyphMetrics> glyphs = {domain::GlyphMetrics(), domain::GlyphMetrics(), domain::GlyphMetrics(),
                                                domain::GlyphMetrics()};
    auto face1 = domain::Face(domain::FixedColor::White, domain::FixedColor::Black, {});
    std::vector<domain::FaceSpan> face_spans = {domain::FaceSpan(face1, 0)};

    SECTION("Empty RenderLine has size zero")
    {
        REQUIRE(domain::RenderLine({}, {}).size() == 0);
    }

    SECTION("RenderLine size is glyphs size not face_spans size")
    {
        REQUIRE(domain::RenderLine(glyphs, face_spans).size() == 4);
    }
}

TEST_CASE("RenderLine width", "[RenderLine]")
{
    auto glyph_resolver = GlyphResolverMock(10);

    auto renderLineFromString = [&glyph_resolver](std::string string) {
        auto line = domain::Line({domain::Atom(
            domain::CodepointString(string), domain::Face(domain::FixedColor::White, domain::FixedColor::Black, {}))});

        return domain::RenderLine(line, glyph_resolver);
    };

    SECTION("Empty RenderLine has zero width")
    {
        REQUIRE(renderLineFromString("").width() == 0);
    }

    SECTION("RenderLines with multiple glyphs")
    {
        REQUIRE(renderLineFromString("12345").width() == 50);
        REQUIRE(renderLineFromString("1234567890").width() == 100);
    }
}

TEST_CASE("RenderLine height", "[RenderLine]")
{
    std::vector<domain::GlyphMetrics> glyphs = {domain::GlyphMetrics{
                                                    'A',
                                                    domain::UIVec2{20, 30},
                                                    domain::IVec2{0, 0},
                                                    20,
                                                },
                                                domain::GlyphMetrics{
                                                    'B',
                                                    domain::UIVec2{20, 50},
                                                    domain::IVec2{0, 0},
                                                    20,
                                                },
                                                domain::GlyphMetrics{
                                                    'C',
                                                    domain::UIVec2{20, 20},
                                                    domain::IVec2{0, 0},
                                                    20,
                                                }};

    std::vector<domain::FaceSpan> face_spans = {
        domain::FaceSpan(domain::Face(domain::FixedColor::White, domain::FixedColor::Black, {}), 0)};

    SECTION("Empty RenderLine has zero height")
    {
        domain::RenderLine empty_render_line({}, {});
        REQUIRE(empty_render_line.height() == 0);
    }

    SECTION("RenderLine with multiple glyphs")
    {
        domain::RenderLine render_line(glyphs, face_spans);
        REQUIRE(render_line.height() == 50);
    }
}

TEST_CASE("RenderLine truncate", "[RenderLine]")
{
    auto glyph_resolver = GlyphResolverMock(10);

    auto face1 = domain::Face(domain::FixedColor::White, domain::FixedColor::Black, {});
    auto face2 = domain::Face(domain::FixedColor::Red, domain::FixedColor::Blue, {});

    auto renderLineFromString = [&glyph_resolver](std::string string) {
        auto line = domain::Line({domain::Atom(
            domain::CodepointString(string), domain::Face(domain::FixedColor::White, domain::FixedColor::Black, {}))});

        return domain::RenderLine(line, glyph_resolver);
    };

    SECTION("Empty RenderLine")
    {
        auto line = domain::Line();
        auto render_line = domain::RenderLine(line, glyph_resolver);

        render_line.truncate(50.0f, glyph_resolver);

        REQUIRE(render_line.size() == 0);
        REQUIRE(render_line.width() == 0);
    }

    SECTION("RenderLine.width() < max_width")
    {
        auto line = domain::Line({domain::Atom(domain::CodepointString("ABCDE"), face1)});
        auto render_line = domain::RenderLine(line, glyph_resolver);

        render_line.truncate(100.0f, glyph_resolver);

        REQUIRE(render_line.size() == 5);
        REQUIRE(render_line.width() == 50.0f);
        REQUIRE(render_line.getFaceSpans().size() == 1);
    }

    SECTION("max_width is less than a single glyph width")
    {
        auto line = domain::Line({domain::Atom(domain::CodepointString("ABCDE"), face1)});
        auto render_line = domain::RenderLine(line, glyph_resolver);

        render_line.truncate(5.0f, glyph_resolver);

        REQUIRE(render_line.size() == 0);
        REQUIRE(render_line.width() == 0);
        REQUIRE(render_line.getFaceSpans().size() == 0);
    }

    SECTION("only space for ellipsis")
    {
        auto line = domain::Line({domain::Atom(domain::CodepointString("ABCDE"), face1)});
        auto render_line = domain::RenderLine(line, glyph_resolver);

        render_line.truncate(15.0f, glyph_resolver);

        REQUIRE(render_line.size() == 1);
        REQUIRE(render_line.width() <= 15.0f);
        REQUIRE(render_line.getGlyphs()[0].codepoint == 0x2026);
        REQUIRE(render_line.getFaceSpans().size() == 1);
    }

    SECTION("Single FaceSpan")
    {
        auto line = domain::Line({domain::Atom(domain::CodepointString("ABCDE"), face1)});
        auto render_line = domain::RenderLine(line, glyph_resolver);

        render_line.truncate(35.0f, glyph_resolver);

        REQUIRE(render_line.size() == 3);
        REQUIRE(render_line.width() <= 35.0f);
        REQUIRE(render_line.getFaceSpans().size() == 1);
    }

    SECTION("Multiple FaceSpan")
    {
        auto line = domain::Line({domain::Atom(domain::CodepointString("ABCDE"), face1),
                                  domain::Atom(domain::CodepointString("FGH"), face2)});
        auto render_line = domain::RenderLine(line, glyph_resolver);

        render_line.truncate(75.0f, glyph_resolver);

        REQUIRE(render_line.size() == 7);
        REQUIRE(render_line.width() <= 75.0f);
        REQUIRE(render_line.getGlyphs()[render_line.size() - 1].codepoint == 0x2026);
        REQUIRE(render_line.getFaceSpans().size() == 2);
    }

    SECTION("Multiple FaceSpan DELETE")
    {
        auto line = domain::Line({domain::Atom(domain::CodepointString("ABCDE"), face1),
                                  domain::Atom(domain::CodepointString("FGH"), face2)});
        auto render_line = domain::RenderLine(line, glyph_resolver);

        render_line.truncate(35.0f, glyph_resolver);

        REQUIRE(render_line.size() == 3);
        REQUIRE(render_line.width() <= 35.0f);
        REQUIRE(render_line.getGlyphs()[render_line.size() - 1].codepoint == 0x2026);
        REQUIRE(render_line.getFaceSpans().size() == 1);
    }

    SECTION("Multiple FaceSpans on border between FaceSpans")
    {
        auto line = domain::Line({domain::Atom(domain::CodepointString("ABCDE"), face1),
                                  domain::Atom(domain::CodepointString("FGH"), face2)});
        auto render_line = domain::RenderLine(line, glyph_resolver);

        render_line.truncate(50.0f, glyph_resolver);

        REQUIRE(render_line.size() == 5);
        REQUIRE(render_line.width() <= 50.0f);
        REQUIRE(render_line.getGlyphs()[render_line.size() - 1].codepoint == 0x2026);
        REQUIRE(render_line.getFaceSpans().size() == 1);
    }

    SECTION("RenderLine.width() == max_width")
    {
        auto line = domain::Line({domain::Atom(domain::CodepointString("ABCDE"), face1)});
        auto render_line = domain::RenderLine(line, glyph_resolver);

        render_line.truncate(50.0f, glyph_resolver);

        REQUIRE(render_line.size() == 5);
        REQUIRE(render_line.width() == 50.0f);
        REQUIRE(render_line.getGlyphs()[render_line.size() - 1].codepoint == 0x2026);
        REQUIRE(render_line.getFaceSpans().size() == 1);
    }
}
TEST_CASE("RenderLine faceSpanIteratorFromIndex", "[RenderLine]")
{

    auto face1 = domain::Face(domain::FixedColor::White, domain::FixedColor::Black, {});
    auto face2 = domain::Face(domain::FixedColor::Red, domain::FixedColor::Blue, {});

    std::vector<domain::GlyphMetrics> glyphs;
    glyphs.reserve(35);
    for (int i = 0; i < 35; i++)
    {
        glyphs.emplace_back(domain::GlyphMetrics{
            'A',
            domain::UIVec2{20, 30},
            domain::IVec2{0, 0},
            20,
        });
    }

    std::vector<domain::FaceSpan> spans({
        domain::FaceSpan(face1, 0),
        domain::FaceSpan(face1, 3),
        domain::FaceSpan(face1, 5),
        domain::FaceSpan(face1, 6),
        domain::FaceSpan(face1, 10),
        domain::FaceSpan(face1, 20),
        domain::FaceSpan(face1, 25),
        domain::FaceSpan(face1, 30),
    });

    domain::RenderLine render_line(glyphs, spans);

    REQUIRE(render_line.faceSpanIteratorFromIndex(0).base() == &render_line.getFaceSpans()[0]);
    REQUIRE(render_line.faceSpanIteratorFromIndex(2).base() == &render_line.getFaceSpans()[0]);
    REQUIRE(render_line.faceSpanIteratorFromIndex(5).base() == &render_line.getFaceSpans()[2]);
    REQUIRE(render_line.faceSpanIteratorFromIndex(5).base() == &render_line.getFaceSpans()[2]);
    REQUIRE(render_line.faceSpanIteratorFromIndex(10).base() == &render_line.getFaceSpans()[4]);
    REQUIRE(render_line.faceSpanIteratorFromIndex(21).base() == &render_line.getFaceSpans()[5]);
    REQUIRE(render_line.faceSpanIteratorFromIndex(29).base() == &render_line.getFaceSpans()[6]);
    REQUIRE(render_line.faceSpanIteratorFromIndex(32).base() == &render_line.getFaceSpans()[7]);
}
