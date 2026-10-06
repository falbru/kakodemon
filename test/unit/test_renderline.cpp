#include "domain/atom.hpp"
#include "domain/codepointstring.hpp"
#include "domain/color.hpp"
#include "domain/face.hpp"
#include "domain/geometry.hpp"
#include "domain/ports/font.hpp"
#include "domain/renderline.hpp"
#include "domain/span.hpp"
#include "mock_faceresolver.hpp"
#include "mock_font.hpp"
#include "mock_glyphresolver.hpp"
#include <catch2/catch_test_macros.hpp>

const domain::RGBAColor RGBACOLOR_WHITE = domain::RGBAColor{1, 1, 1, 1};
const domain::RGBAColor RGBACOLOR_BLACK = domain::RGBAColor{0, 0, 0, 1};
const domain::RGBAColor RGBACOLOR_RED = domain::RGBAColor{1, 0, 0, 1};

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

    domain::ResolvedFace face1(RGBACOLOR_WHITE, RGBACOLOR_BLACK, {});
    FontMock font1;

    std::vector<domain::Span<domain::ResolvedFace>> face_spans = {domain::Span<domain::ResolvedFace>(face1, 0)};
    std::vector<domain::Span<domain::Font *>> font_spans = {domain::Span<domain::Font *>(&font1, 0)};

    SECTION("Empty")
    {
        REQUIRE_NOTHROW(domain::RenderLine({}, {}, {}));

        domain::RenderLine render_line({}, {}, {});

        REQUIRE(render_line.getGlyphs().size() == 0);
        REQUIRE(render_line.getFaceSpans().size() == 0);
        REQUIRE(render_line.getFontSpans().size() == 0);
    }

    SECTION("Handle sorted face spans")
    {
        domain::RenderLine render_line(glyphs, face_spans, font_spans);

        REQUIRE(render_line.getGlyphs().size() == 3);
        REQUIRE(render_line.getGlyphs()[0].codepoint == 'A');
        REQUIRE(render_line.getGlyphs()[1].codepoint == 'B');
        REQUIRE(render_line.getGlyphs()[2].codepoint == 'C');
        REQUIRE(render_line.getFaceSpans().size() == 1);
        REQUIRE(render_line.getFaceSpans()[0].value == face1);
        REQUIRE(render_line.getFaceSpans()[0].start_index == 0);
    }

    SECTION("Throw when face size is empty for non-empty glyphs")
    {
        REQUIRE_THROWS(domain::RenderLine(glyphs, {}, font_spans));
    }

    SECTION("Throw when face size is non-empty for empty glyphs")
    {
        REQUIRE_THROWS(domain::RenderLine({}, face_spans, {}));
    }

    SECTION("Handle unsorted face spans")
    {
        domain::RenderLine render_line(glyphs,
                                       {domain::Span<domain::ResolvedFace>(face1, 2),
                                        domain::Span<domain::ResolvedFace>(face1, 0),
                                        domain::Span<domain::ResolvedFace>(face1, 1)},
                                       font_spans);

        REQUIRE(render_line.getFaceSpans().size() == 3);
        REQUIRE(render_line.getFaceSpans()[0].start_index == 0);
        REQUIRE(render_line.getFaceSpans()[1].start_index == 1);
        REQUIRE(render_line.getFaceSpans()[2].start_index == 2);
    }

    SECTION("Throw when face span start_index that exceeds glyph size")
    {
        REQUIRE_THROWS(domain::RenderLine(
            glyphs, {domain::Span<domain::ResolvedFace>(face1, 10), domain::Span<domain::ResolvedFace>(face1, 0)},
            font_spans));
    }

    SECTION("Throw when font size is empty for non-empty glyphs")
    {
        REQUIRE_THROWS(domain::RenderLine(glyphs, face_spans, {}));
    }

    SECTION("Throw when font size is non-empty for empty glyphs")
    {
        REQUIRE_THROWS(domain::RenderLine({}, {}, font_spans));
    }

    SECTION("Handle unsorted font spans")
    {
        domain::RenderLine render_line(glyphs, face_spans,
                                       {domain::Span<domain::Font *>(&font1, 2),
                                        domain::Span<domain::Font *>(&font1, 0),
                                        domain::Span<domain::Font *>(&font1, 1)});

        REQUIRE(render_line.getFontSpans().size() == 3);
        REQUIRE(render_line.getFontSpans()[0].start_index == 0);
        REQUIRE(render_line.getFontSpans()[1].start_index == 1);
        REQUIRE(render_line.getFontSpans()[2].start_index == 2);
    }

    SECTION("Throw when font span start_index that exceeds glyph size")
    {
        REQUIRE_THROWS(domain::RenderLine(
            glyphs, face_spans, {domain::Span<domain::Font *>(&font1, 10), domain::Span<domain::Font *>(&font1, 0)}));
    }
}

TEST_CASE("RenderLine constructor with line and glyph resolver", "[RenderLine]")
{
    FontMock font1;
    FontMock font2;

    auto glyph_resolver = GlyphResolverMock(10, &font1).withResolveFont([&font1, &font2](domain::Codepoint c) {
        if (c == 'X')
        {
            return &font2;
        }
        else
        {
            return &font1;
        }
    });

    auto face1 = domain::Face(RGBACOLOR_WHITE, RGBACOLOR_BLACK, {});
    auto face2 = domain::Face(RGBACOLOR_RED, RGBACOLOR_BLACK, {});

    auto resolved_face1 = domain::ResolvedFace(RGBACOLOR_WHITE, RGBACOLOR_BLACK, {});
    auto resolved_face2 = domain::ResolvedFace(RGBACOLOR_RED, RGBACOLOR_BLACK, {});

    auto face_resolver = FaceResolverMock([face1, face2, resolved_face1, resolved_face2](const domain::Face &face) {
        if (face == face1)
        {
            return resolved_face1;
        }

        if (face == face2)
        {
            return resolved_face2;
        }

        return domain::ResolvedFace({}, {}, {});
    });

    SECTION("Empty line")
    {
        domain::Line line;
        domain::RenderLine render_line(line, glyph_resolver, face_resolver);

        REQUIRE(render_line.getGlyphs().size() == 0);
        REQUIRE(render_line.getFaceSpans().size() == 0);
        REQUIRE(render_line.getFontSpans().size() == 0);
    }

    SECTION("Line with one atom")
    {
        domain::Line line({domain::Atom(domain::CodepointString("ABC"), face1)});
        domain::RenderLine render_line(line, glyph_resolver, face_resolver);

        REQUIRE(render_line.getGlyphs().size() == 3);
        REQUIRE(render_line.getGlyphs()[0].codepoint == 'A');
        REQUIRE(render_line.getGlyphs()[1].codepoint == 'B');
        REQUIRE(render_line.getGlyphs()[2].codepoint == 'C');
        REQUIRE(render_line.getFaceSpans().size() == 1);
        REQUIRE(render_line.getFaceSpans()[0].value == resolved_face1);
        REQUIRE(render_line.getFaceSpans()[0].start_index == 0);
        REQUIRE(render_line.getFontSpans().size() == 1);
        REQUIRE(render_line.getFontSpans()[0].value == &font1);
        REQUIRE(render_line.getFontSpans()[0].start_index == 0);
    }

    SECTION("Line with multiple atoms")
    {
        domain::Line line(
            {domain::Atom(domain::CodepointString("ABC"), face1), domain::Atom(domain::CodepointString("DEF"), face2)});
        domain::RenderLine render_line(line, glyph_resolver, face_resolver);

        REQUIRE(render_line.getGlyphs().size() == 6);
        REQUIRE(render_line.getGlyphs()[0].codepoint == 'A');
        REQUIRE(render_line.getGlyphs()[1].codepoint == 'B');
        REQUIRE(render_line.getGlyphs()[2].codepoint == 'C');
        REQUIRE(render_line.getGlyphs()[3].codepoint == 'D');
        REQUIRE(render_line.getGlyphs()[4].codepoint == 'E');
        REQUIRE(render_line.getGlyphs()[5].codepoint == 'F');
        REQUIRE(render_line.getFaceSpans().size() == 2);
        REQUIRE(render_line.getFaceSpans()[0].value == resolved_face1);
        REQUIRE(render_line.getFaceSpans()[0].start_index == 0);
        REQUIRE(render_line.getFaceSpans()[1].value == resolved_face2);
        REQUIRE(render_line.getFaceSpans()[1].start_index == 3);
        REQUIRE(render_line.getFontSpans().size() == 1);
        REQUIRE(render_line.getFontSpans()[0].value == &font1);
        REQUIRE(render_line.getFontSpans()[0].start_index == 0);
    }

    SECTION("Line with multiple fonts")
    {
        domain::Line line(
            {domain::Atom(domain::CodepointString("ABX"), face1), domain::Atom(domain::CodepointString("XEX"), face2)});
        domain::RenderLine render_line(line, glyph_resolver, face_resolver);

        REQUIRE(render_line.getGlyphs().size() == 6);
        REQUIRE(render_line.getGlyphs()[0].codepoint == 'A');
        REQUIRE(render_line.getGlyphs()[1].codepoint == 'B');
        REQUIRE(render_line.getGlyphs()[2].codepoint == 'X');
        REQUIRE(render_line.getGlyphs()[3].codepoint == 'X');
        REQUIRE(render_line.getGlyphs()[4].codepoint == 'E');
        REQUIRE(render_line.getGlyphs()[5].codepoint == 'X');
        REQUIRE(render_line.getFaceSpans().size() == 2);
        REQUIRE(render_line.getFaceSpans()[0].value == resolved_face1);
        REQUIRE(render_line.getFaceSpans()[0].start_index == 0);
        REQUIRE(render_line.getFaceSpans()[1].value == resolved_face2);
        REQUIRE(render_line.getFaceSpans()[1].start_index == 3);
        REQUIRE(render_line.getFontSpans().size() == 4);
        REQUIRE(render_line.getFontSpans()[0].value == &font1);
        REQUIRE(render_line.getFontSpans()[0].start_index == 0);
        REQUIRE(render_line.getFontSpans()[1].value == &font2);
        REQUIRE(render_line.getFontSpans()[1].start_index == 2);
        REQUIRE(render_line.getFontSpans()[2].value == &font1);
        REQUIRE(render_line.getFontSpans()[2].start_index == 4);
        REQUIRE(render_line.getFontSpans()[3].value == &font2);
        REQUIRE(render_line.getFontSpans()[3].start_index == 5);
    }
}

TEST_CASE("RenderLine size", "[RenderLine]")
{
    std::vector<domain::GlyphMetrics> glyphs = {domain::GlyphMetrics(), domain::GlyphMetrics(), domain::GlyphMetrics(),
                                                domain::GlyphMetrics()};
    auto face1 = domain::ResolvedFace(RGBACOLOR_WHITE, RGBACOLOR_BLACK, {});
    std::vector<domain::Span<domain::ResolvedFace>> face_spans = {domain::Span<domain::ResolvedFace>(face1, 0)};

    FontMock font1;
    std::vector<domain::Span<domain::Font *>> font_spans = {domain::Span<domain::Font *>(&font1, 0)};

    SECTION("Empty RenderLine has size zero")
    {
        REQUIRE(domain::RenderLine({}, {}, {}).size() == 0);
    }

    SECTION("RenderLine size is glyphs size not face_spans size")
    {
        REQUIRE(domain::RenderLine(glyphs, face_spans, font_spans).size() == 4);
    }
}

TEST_CASE("RenderLine width", "[RenderLine]")
{
    GlyphResolverMock glyph_resolver(10, nullptr);
    FaceResolverMock face_resolver(domain::ResolvedFace{RGBACOLOR_WHITE, RGBACOLOR_BLACK});

    auto renderLineFromString = [&glyph_resolver, &face_resolver](std::string string) {
        auto line = domain::Line({domain::Atom(
            domain::CodepointString(string), domain::Face(domain::NamedColor::White, domain::NamedColor::Black, {}))});

        return domain::RenderLine(line, glyph_resolver, face_resolver);
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

    std::vector<domain::Span<domain::ResolvedFace>> face_spans = {
        domain::Span<domain::ResolvedFace>(domain::ResolvedFace(RGBACOLOR_WHITE, RGBACOLOR_BLACK, {}), 0)};

    FontMock font1;
    std::vector<domain::Span<domain::Font *>> font_spans = {domain::Span<domain::Font *>(&font1, 0)};

    SECTION("Empty RenderLine has zero height")
    {
        domain::RenderLine empty_render_line({}, {}, {});
        REQUIRE(empty_render_line.height() == 0);
    }

    SECTION("RenderLine with multiple glyphs")
    {
        domain::RenderLine render_line(glyphs, face_spans, font_spans);
        REQUIRE(render_line.height() == 50);
    }
}

TEST_CASE("RenderLine truncate", "[RenderLine]")
{
    FontMock font1;
    FontMock font2;

    auto glyph_resolver = GlyphResolverMock(10, &font1).withResolveFont([&font1, &font2](domain::Codepoint c) {
        if (c == 'X')
        {
            return &font1;
        }

        return &font2;
    });

    auto face1 = domain::Face(domain::NamedColor::White, domain::NamedColor::Black, {});
    auto face2 = domain::Face(domain::NamedColor::Red, domain::NamedColor::Black, {});

    auto resolved_face1 = domain::ResolvedFace(RGBACOLOR_WHITE, RGBACOLOR_BLACK, {});
    auto resolved_face2 = domain::ResolvedFace(RGBACOLOR_RED, RGBACOLOR_BLACK, {});

    auto face_resolver = FaceResolverMock([face1, face2, resolved_face1, resolved_face2](const domain::Face &face) {
        if (face == face1)
        {
            return resolved_face1;
        }

        if (face == face2)
        {
            return resolved_face2;
        }

        return domain::ResolvedFace({}, {}, {});
    });

    auto renderLineFromString = [&glyph_resolver, &face_resolver](std::string string) {
        auto line = domain::Line({domain::Atom(
            domain::CodepointString(string), domain::Face(domain::NamedColor::White, domain::NamedColor::Black, {}))});

        return domain::RenderLine(line, glyph_resolver, face_resolver);
    };

    SECTION("Empty RenderLine")
    {
        auto line = domain::Line();
        auto render_line = domain::RenderLine(line, glyph_resolver, face_resolver);

        render_line.truncate(50.0f, glyph_resolver);

        REQUIRE(render_line.size() == 0);
        REQUIRE(render_line.width() == 0);
    }

    SECTION("RenderLine.width() < max_width")
    {
        auto line = domain::Line({domain::Atom(domain::CodepointString("ABCDE"), face1)});
        auto render_line = domain::RenderLine(line, glyph_resolver, face_resolver);

        render_line.truncate(100.0f, glyph_resolver);

        REQUIRE(render_line.size() == 5);
        REQUIRE(render_line.width() == 50.0f);
        REQUIRE(render_line.getFaceSpans().size() == 1);
        REQUIRE(render_line.getFontSpans().size() == 1);
    }

    SECTION("max_width is less than a single glyph width")
    {
        auto line = domain::Line({domain::Atom(domain::CodepointString("ABCDE"), face1)});
        auto render_line = domain::RenderLine(line, glyph_resolver, face_resolver);

        render_line.truncate(5.0f, glyph_resolver);

        REQUIRE(render_line.size() == 0);
        REQUIRE(render_line.width() == 0);
        REQUIRE(render_line.getFaceSpans().size() == 0);
        REQUIRE(render_line.getFontSpans().size() == 0);
    }

    SECTION("only space for ellipsis")
    {
        auto line = domain::Line({domain::Atom(domain::CodepointString("ABCDE"), face1)});
        auto render_line = domain::RenderLine(line, glyph_resolver, face_resolver);

        render_line.truncate(15.0f, glyph_resolver);

        REQUIRE(render_line.size() == 1);
        REQUIRE(render_line.width() <= 15.0f);
        REQUIRE(render_line.getGlyphs()[0].codepoint == 0x2026);
        REQUIRE(render_line.getFaceSpans().size() == 1);
        REQUIRE(render_line.getFontSpans().size() == 1);
    }

    SECTION("Single FaceSpan")
    {
        auto line = domain::Line({domain::Atom(domain::CodepointString("ABCDE"), face1)});
        auto render_line = domain::RenderLine(line, glyph_resolver, face_resolver);

        render_line.truncate(35.0f, glyph_resolver);

        REQUIRE(render_line.size() == 3);
        REQUIRE(render_line.width() <= 35.0f);
        REQUIRE(render_line.getFaceSpans().size() == 1);
        REQUIRE(render_line.getFontSpans().size() == 1);
    }

    SECTION("Multiple FaceSpan")
    {
        auto line = domain::Line({domain::Atom(domain::CodepointString("ABCDE"), face1),
                                  domain::Atom(domain::CodepointString("FGH"), face2)});
        auto render_line = domain::RenderLine(line, glyph_resolver, face_resolver);

        render_line.truncate(75.0f, glyph_resolver);

        REQUIRE(render_line.size() == 7);
        REQUIRE(render_line.width() <= 75.0f);
        REQUIRE(render_line.getGlyphs()[render_line.size() - 1].codepoint == 0x2026);
        REQUIRE(render_line.getFaceSpans().size() == 2);
        REQUIRE(render_line.getFontSpans().size() == 1);
    }

    SECTION("FaceSpan gets deleted")
    {
        auto line = domain::Line({domain::Atom(domain::CodepointString("ABCDE"), face1),
                                  domain::Atom(domain::CodepointString("FGH"), face2)});
        auto render_line = domain::RenderLine(line, glyph_resolver, face_resolver);

        render_line.truncate(35.0f, glyph_resolver);

        REQUIRE(render_line.size() == 3);
        REQUIRE(render_line.width() <= 35.0f);
        REQUIRE(render_line.getGlyphs()[render_line.size() - 1].codepoint == 0x2026);
        REQUIRE(render_line.getFaceSpans().size() == 1);
    }

    SECTION("Truncate on border between FaceSpans")
    {
        auto line = domain::Line({domain::Atom(domain::CodepointString("ABCDE"), face1),
                                  domain::Atom(domain::CodepointString("FGH"), face2)});
        auto render_line = domain::RenderLine(line, glyph_resolver, face_resolver);

        render_line.truncate(50.0f, glyph_resolver);

        REQUIRE(render_line.size() == 5);
        REQUIRE(render_line.width() <= 50.0f);
        REQUIRE(render_line.getGlyphs()[render_line.size() - 1].codepoint == 0x2026);
        REQUIRE(render_line.getFaceSpans().size() == 1);
    }

    SECTION("Multiple FontSpan")
    {
        auto line = domain::Line({domain::Atom(domain::CodepointString("ABXXE"), face1),
                                  domain::Atom(domain::CodepointString("FXH"), face2)});
        auto render_line = domain::RenderLine(line, glyph_resolver, face_resolver);

        render_line.truncate(75.0f, glyph_resolver);

        REQUIRE(render_line.size() == 7);
        REQUIRE(render_line.width() <= 75.0f);
        REQUIRE(render_line.getGlyphs()[render_line.size() - 1].codepoint == 0x2026);
        REQUIRE(render_line.getFaceSpans().size() == 2);
        REQUIRE(render_line.getFontSpans().size() == 3);
    }

    SECTION("FontSpan gets deleted")
    {
        auto line = domain::Line({domain::Atom(domain::CodepointString("ABXXE"), face1),
                                  domain::Atom(domain::CodepointString("FXH"), face2)});
        auto render_line = domain::RenderLine(line, glyph_resolver, face_resolver);

        render_line.truncate(45.0f, glyph_resolver);

        REQUIRE(render_line.size() == 4);
        REQUIRE(render_line.width() <= 45.0f);
        REQUIRE(render_line.getGlyphs()[render_line.size() - 1].codepoint == 0x2026);
        REQUIRE(render_line.getFaceSpans().size() == 1);
        REQUIRE(render_line.getFontSpans().size() == 2);
    }

    SECTION("Truncate on border between FontSpans")
    {
        auto line = domain::Line({domain::Atom(domain::CodepointString("ABXXE"), face1),
                                  domain::Atom(domain::CodepointString("FXH"), face2)});
        auto render_line = domain::RenderLine(line, glyph_resolver, face_resolver);

        render_line.truncate(35.0f, glyph_resolver);

        REQUIRE(render_line.size() == 3);
        REQUIRE(render_line.width() <= 35.0f);
        REQUIRE(render_line.getGlyphs()[render_line.size() - 1].codepoint == 0x2026);
        REQUIRE(render_line.getFaceSpans().size() == 1);
        REQUIRE(render_line.getFontSpans().size() == 1);
    }

    SECTION("RenderLine.width() == max_width")
    {
        auto line = domain::Line({domain::Atom(domain::CodepointString("ABCDE"), face1)});
        auto render_line = domain::RenderLine(line, glyph_resolver, face_resolver);

        render_line.truncate(50.0f, glyph_resolver);

        REQUIRE(render_line.size() == 5);
        REQUIRE(render_line.width() == 50.0f);
        REQUIRE(render_line.getGlyphs()[render_line.size() - 1].codepoint == 0x2026);
        REQUIRE(render_line.getFaceSpans().size() == 1);
    }
}

TEST_CASE("RenderLine split", "[RenderLine]")
{
    FontMock font1;
    FontMock font2;

    auto glyph_resolver = GlyphResolverMock(10, &font1).withResolveFont([&font1, &font2](domain::Codepoint c) {
        if (c == 'X')
        {
            return &font2;
        }
        return &font1;
    });

    auto face1 = domain::Face(domain::NamedColor::White, domain::NamedColor::Black, {});
    auto face2 = domain::Face(domain::NamedColor::Red, domain::NamedColor::White, {});

    auto resolved_face1 = domain::ResolvedFace(RGBACOLOR_WHITE, RGBACOLOR_BLACK, {});
    auto resolved_face2 = domain::ResolvedFace(RGBACOLOR_RED, RGBACOLOR_WHITE, {});

    auto face_resolver = FaceResolverMock([face1, face2, resolved_face1, resolved_face2](const domain::Face &face) {
        if (face == face1)
        {
            return resolved_face1;
        }

        if (face == face2)
        {
            return resolved_face2;
        }

        return domain::ResolvedFace({}, {}, {});
    });

    domain::RenderLine line(domain::Line({domain::Atom(domain::CodepointString("HELLO"), face1),
                                          domain::Atom(domain::CodepointString("XWORLD"), face2)}),
                            glyph_resolver, face_resolver);

    SECTION("If start is higher than length")
    {
        auto split = line.split(100, 150);

        REQUIRE(split.size() == 0);
        REQUIRE(split.getFaceSpans().size() == 0);
        REQUIRE(split.getFontSpans().size() == 0);
    }

    SECTION("If start is higher than end")
    {
        auto split = line.split(4, 3);

        REQUIRE(split.size() == 0);
        REQUIRE(split.getFaceSpans().size() == 0);

        split = line.split(3, 3);

        REQUIRE(split.size() == 0);
        REQUIRE(split.getFaceSpans().size() == 0);
        REQUIRE(split.getFontSpans().size() == 0);
    }

    SECTION("Split to one glyph")
    {
        auto split = line.split(3, 4);

        REQUIRE(split.size() == 1);
        REQUIRE(split.getGlyphs()[0].codepoint == 'L');
        REQUIRE(split.getFaceSpans().size() == 1);
        REQUIRE(split.getFaceSpans()[0].start_index == 0);
        REQUIRE(split.getFaceSpans()[0].value == resolved_face1);
        REQUIRE(split.getFontSpans().size() == 1);
        REQUIRE(split.getFontSpans()[0].start_index == 0);
        REQUIRE(split.getFontSpans()[0].value == &font1);
    }

    SECTION("Split to the end")
    {
        auto split = line.split(2, line.size());

        REQUIRE(split.size() == 9);
        REQUIRE(split.getGlyphs()[0].codepoint == 'L');
        REQUIRE(split.getFaceSpans().size() == 2);
        REQUIRE(split.getFaceSpans()[0].start_index == 0);
        REQUIRE(split.getFaceSpans()[1].start_index == 3);
        REQUIRE(split.getFontSpans().size() == 3);
        REQUIRE(split.getFontSpans()[0].start_index == 0);
        REQUIRE(split.getFontSpans()[0].value == &font1);
        REQUIRE(split.getFontSpans()[1].start_index == 3);
        REQUIRE(split.getFontSpans()[1].value == &font2);
        REQUIRE(split.getFontSpans()[2].start_index == 4);
        REQUIRE(split.getFontSpans()[2].value == &font1);
    }

    SECTION("If end is greater than glyph size, cap to glyph size")
    {
        auto split = line.split(2, 100);

        REQUIRE(split.size() == 9);
        REQUIRE(split.getGlyphs()[0].codepoint == 'L');
        REQUIRE(split.getFaceSpans().size() == 2);
        REQUIRE(split.getFaceSpans()[0].start_index == 0);
        REQUIRE(split.getFaceSpans()[1].start_index == 3);
        REQUIRE(split.getFontSpans().size() == 3);
        REQUIRE(split.getFontSpans()[0].start_index == 0);
        REQUIRE(split.getFontSpans()[0].value == &font1);
        REQUIRE(split.getFontSpans()[1].start_index == 3);
        REQUIRE(split.getFontSpans()[1].value == &font2);
        REQUIRE(split.getFontSpans()[2].start_index == 4);
        REQUIRE(split.getFontSpans()[2].value == &font1);
    }
}
