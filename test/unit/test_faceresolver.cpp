#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "domain/color.hpp"
#include "domain/face.hpp"
#include "domain/faceresolver.hpp"

TEST_CASE("FaceResolver constructor", "[FaceResolver]")
{
    SECTION("constructor with all parameters")
    {
        domain::DefaultFace default_face(domain::NamedColor::Black, domain::NamedColor::White);
        domain::ColorOverrides overrides;
        domain::Face fallback_face(domain::NamedColor::Red, domain::NamedColor::Blue);

        domain::FaceResolver resolver(fallback_face, default_face, overrides);

        domain::Face test_face(domain::DefaultColor{}, domain::DefaultColor{});
        domain::ResolvedFace resolved = resolver.resolve(test_face);

        domain::RGBAColor expected_bg = domain::getRGBAColor(domain::NamedColor::Red);
        domain::RGBAColor expected_fg = domain::getRGBAColor(domain::NamedColor::Blue);

        REQUIRE(resolved.getBg().r == expected_bg.r);
        REQUIRE(resolved.getBg().g == expected_bg.g);
        REQUIRE(resolved.getBg().b == expected_bg.b);
        REQUIRE(resolved.getFg().r == expected_fg.r);
        REQUIRE(resolved.getFg().g == expected_fg.g);
        REQUIRE(resolved.getFg().b == expected_fg.b);
    }

    SECTION("constructor with nullopt fallback face")
    {
        domain::DefaultFace default_face(domain::NamedColor::Black, domain::NamedColor::White);
        domain::ColorOverrides overrides;

        domain::FaceResolver resolver(std::nullopt, default_face, overrides);
        domain::Face test_face(domain::DefaultColor{}, domain::DefaultColor{});
        domain::ResolvedFace resolved = resolver.resolve(test_face);

        domain::RGBAColor expected_bg = default_face.resolveBg(overrides);
        domain::RGBAColor expected_fg = default_face.resolveFg(overrides);

        REQUIRE(resolved.getBg().r == expected_bg.r);
        REQUIRE(resolved.getBg().g == expected_bg.g);
        REQUIRE(resolved.getBg().b == expected_bg.b);
        REQUIRE(resolved.getFg().r == expected_fg.r);
        REQUIRE(resolved.getFg().g == expected_fg.g);
        REQUIRE(resolved.getFg().b == expected_fg.b);
    }
}

TEST_CASE("FaceResolver resolve with fallback face", "[FaceResolver][resolve]")
{
    domain::DefaultFace default_face(domain::NamedColor::Black, domain::NamedColor::White);
    domain::Face fallback_face(domain::NamedColor::Cyan, domain::NamedColor::Magenta, {domain::Attribute::Dim});
    domain::ColorOverrides overrides;
    domain::FaceResolver resolver(fallback_face, default_face, overrides);

    SECTION("resolve uses fallback face when provided")
    {
        domain::Face face(domain::DefaultColor{}, domain::DefaultColor{});
        domain::ResolvedFace resolved = resolver.resolve(face);

        domain::RGBAColor expected_bg = domain::getRGBAColor(domain::NamedColor::Cyan);
        domain::RGBAColor expected_fg = domain::getRGBAColor(domain::NamedColor::Magenta);

        REQUIRE(resolved.getBg().r == expected_bg.r);
        REQUIRE(resolved.getBg().g == expected_bg.g);
        REQUIRE(resolved.getBg().b == expected_bg.b);
        REQUIRE(resolved.getFg().r == expected_fg.r);
        REQUIRE(resolved.getFg().g == expected_fg.g);
        REQUIRE(resolved.getFg().b == expected_fg.b);

        REQUIRE_FALSE(resolved.hasAttribute(domain::Attribute::Dim));
    }

    SECTION("resolve uses face's explicit colors over fallback")
    {
        domain::Face face(domain::NamedColor::Red, domain::NamedColor::Blue, {domain::Attribute::Bold});
        domain::ResolvedFace resolved = resolver.resolve(face);

        domain::RGBAColor expected_bg = domain::getRGBAColor(domain::NamedColor::Red);
        domain::RGBAColor expected_fg = domain::getRGBAColor(domain::NamedColor::Blue);

        REQUIRE(resolved.getBg().r == expected_bg.r);
        REQUIRE(resolved.getBg().g == expected_bg.g);
        REQUIRE(resolved.getBg().b == expected_bg.b);
        REQUIRE(resolved.getFg().r == expected_fg.r);
        REQUIRE(resolved.getFg().g == expected_fg.g);
        REQUIRE(resolved.getFg().b == expected_fg.b);

        REQUIRE(resolved.hasAttribute(domain::Attribute::Bold));
        REQUIRE_FALSE(resolved.hasAttribute(domain::Attribute::Dim));
    }

    SECTION("resolve uses fallback for DefaultColor, explicit for NamedColor")
    {
        domain::Face face(domain::DefaultColor{}, domain::NamedColor::Yellow);
        domain::ResolvedFace resolved = resolver.resolve(face);

        domain::RGBAColor expected_bg = domain::getRGBAColor(domain::NamedColor::Cyan);
        domain::RGBAColor expected_fg = domain::getRGBAColor(domain::NamedColor::Yellow);

        REQUIRE(resolved.getBg().r == expected_bg.r);
        REQUIRE(resolved.getBg().g == expected_bg.g);
        REQUIRE(resolved.getBg().b == expected_bg.b);
        REQUIRE(resolved.getFg().r == expected_fg.r);
        REQUIRE(resolved.getFg().g == expected_fg.g);
        REQUIRE(resolved.getFg().b == expected_fg.b);
    }

    SECTION("resolve uses default face when fallback also has DefaultColor")
    {
        domain::Face fallback_with_default(domain::DefaultColor{}, domain::NamedColor::Red);
        domain::FaceResolver resolver2(fallback_with_default, default_face, overrides);

        domain::Face face(domain::DefaultColor{}, domain::NamedColor::Blue);
        domain::ResolvedFace resolved = resolver2.resolve(face);

        domain::RGBAColor expected_bg = default_face.resolveBg(overrides);
        domain::RGBAColor expected_fg = domain::getRGBAColor(domain::NamedColor::Blue);

        REQUIRE(resolved.getBg().r == expected_bg.r);
        REQUIRE(resolved.getBg().g == expected_bg.g);
        REQUIRE(resolved.getBg().b == expected_bg.b);
        REQUIRE(resolved.getFg().r == expected_fg.r);
        REQUIRE(resolved.getFg().g == expected_fg.g);
        REQUIRE(resolved.getFg().b == expected_fg.b);
    }
}

TEST_CASE("FaceResolver resolve without fallback face", "[FaceResolver][resolve]")
{
    domain::DefaultFace default_face(domain::NamedColor::Black, domain::NamedColor::White);
    domain::ColorOverrides overrides;
    domain::FaceResolver resolver(std::nullopt, default_face, overrides);

    SECTION("resolve uses default face when no fallback")
    {
        domain::Face face(domain::DefaultColor{}, domain::DefaultColor{});
        domain::ResolvedFace resolved = resolver.resolve(face);

        domain::RGBAColor expected_bg = default_face.resolveBg(overrides);
        domain::RGBAColor expected_fg = default_face.resolveFg(overrides);

        REQUIRE(resolved.getBg().r == expected_bg.r);
        REQUIRE(resolved.getBg().g == expected_bg.g);
        REQUIRE(resolved.getBg().b == expected_bg.b);
        REQUIRE(resolved.getFg().r == expected_fg.r);
        REQUIRE(resolved.getFg().g == expected_fg.g);
        REQUIRE(resolved.getFg().b == expected_fg.b);
    }

    SECTION("resolve uses explicit colors over default")
    {
        domain::Face face(domain::NamedColor::Red, domain::NamedColor::Blue,
                          {domain::Attribute::Bold, domain::Attribute::Italic});
        domain::ResolvedFace resolved = resolver.resolve(face);

        domain::RGBAColor expected_bg = domain::getRGBAColor(domain::NamedColor::Red);
        domain::RGBAColor expected_fg = domain::getRGBAColor(domain::NamedColor::Blue);

        REQUIRE(resolved.getBg().r == expected_bg.r);
        REQUIRE(resolved.getBg().g == expected_bg.g);
        REQUIRE(resolved.getBg().b == expected_bg.b);
        REQUIRE(resolved.getFg().r == expected_fg.r);
        REQUIRE(resolved.getFg().g == expected_fg.g);
        REQUIRE(resolved.getFg().b == expected_fg.b);

        REQUIRE(resolved.hasAttribute(domain::Attribute::Bold));
        REQUIRE(resolved.hasAttribute(domain::Attribute::Italic));
    }

    SECTION("resolve mixed DefaultColor and explicit")
    {
        domain::Face face(domain::DefaultColor{}, domain::NamedColor::Yellow);
        domain::ResolvedFace resolved = resolver.resolve(face);

        domain::RGBAColor expected_bg = default_face.resolveBg(overrides);
        domain::RGBAColor expected_fg = domain::getRGBAColor(domain::NamedColor::Yellow);

        REQUIRE(resolved.getBg().r == expected_bg.r);
        REQUIRE(resolved.getBg().g == expected_bg.g);
        REQUIRE(resolved.getBg().b == expected_bg.b);
        REQUIRE(resolved.getFg().r == expected_fg.r);
        REQUIRE(resolved.getFg().g == expected_fg.g);
        REQUIRE(resolved.getFg().b == expected_fg.b);
    }
}

TEST_CASE("FaceResolver with color overrides", "[FaceResolver][resolve][ColorOverrides]")
{
    domain::ColorOverrides overrides;
    overrides[domain::NamedColor::Red] = domain::RGBAColor{0.9f, 0.1f, 0.1f, 1.0f};
    overrides[domain::NamedColor::Blue] = domain::RGBAColor{0.1f, 0.1f, 0.9f, 1.0f};
    overrides[domain::NamedColor::Cyan] = domain::RGBAColor{0.1f, 0.9f, 0.9f, 1.0f};

    domain::DefaultFace default_face(domain::NamedColor::Black, domain::NamedColor::White);
    domain::Face fallback_face(domain::NamedColor::Cyan, domain::NamedColor::Magenta);
    domain::FaceResolver resolver(fallback_face, default_face, overrides);

    SECTION("resolve applies color overrides to face colors")
    {
        domain::Face face(domain::NamedColor::Red, domain::NamedColor::Blue);
        domain::ResolvedFace resolved = resolver.resolve(face);

        REQUIRE(resolved.getBg().r == 0.9f);
        REQUIRE(resolved.getBg().g == 0.1f);
        REQUIRE(resolved.getBg().b == 0.1f);
        REQUIRE(resolved.getFg().r == 0.1f);
        REQUIRE(resolved.getFg().g == 0.1f);
        REQUIRE(resolved.getFg().b == 0.9f);
    }

    SECTION("resolve applies color overrides to fallback colors")
    {
        domain::Face face(domain::DefaultColor{}, domain::DefaultColor{});
        domain::ResolvedFace resolved = resolver.resolve(face);

        REQUIRE(resolved.getBg().r == 0.1f);
        REQUIRE(resolved.getBg().g == 0.9f);
        REQUIRE(resolved.getBg().b == 0.9f);
        REQUIRE(resolved.getFg().r == 1.0f);
        REQUIRE(resolved.getFg().g == 0.0f);
        REQUIRE(resolved.getFg().b == 1.0f);
    }

    SECTION("resolve applies color overrides to default face colors")
    {
        domain::FaceResolver resolver_no_fallback(std::nullopt, default_face, overrides);
        domain::Face face(domain::DefaultColor{}, domain::DefaultColor{});
        domain::ResolvedFace resolved = resolver_no_fallback.resolve(face);

        domain::RGBAColor expected_bg = domain::getRGBAColor(domain::NamedColor::Black);
        domain::RGBAColor expected_fg = domain::getRGBAColor(domain::NamedColor::White);

        REQUIRE(resolved.getBg().r == expected_bg.r);
        REQUIRE(resolved.getBg().g == expected_bg.g);
        REQUIRE(resolved.getBg().b == expected_bg.b);
        REQUIRE(resolved.getFg().r == expected_fg.r);
        REQUIRE(resolved.getFg().g == expected_fg.g);
        REQUIRE(resolved.getFg().b == expected_fg.b);
    }

    SECTION("resolve ignores overrides for RGBAColor")
    {
        domain::RGBAColor custom_bg{0.2f, 0.3f, 0.4f, 1.0f};
        domain::RGBAColor custom_fg{0.5f, 0.6f, 0.7f, 1.0f};
        domain::Face face(custom_bg, custom_fg);
        domain::ResolvedFace resolved = resolver.resolve(face);

        REQUIRE(resolved.getBg().r == 0.2f);
        REQUIRE(resolved.getBg().g == 0.3f);
        REQUIRE(resolved.getBg().b == 0.4f);
        REQUIRE(resolved.getFg().r == 0.5f);
        REQUIRE(resolved.getFg().g == 0.6f);
        REQUIRE(resolved.getFg().b == 0.7f);
    }

    SECTION("resolve uses overrides with mixed fallback and default")
    {
        domain::Face fallback_with_default(domain::NamedColor::Cyan, domain::DefaultColor{});
        domain::FaceResolver resolver2(fallback_with_default, default_face, overrides);

        domain::Face face(domain::DefaultColor{}, domain::NamedColor::Red);
        domain::ResolvedFace resolved = resolver2.resolve(face);

        REQUIRE(resolved.getBg().r == 0.1f);
        REQUIRE(resolved.getBg().g == 0.9f);
        REQUIRE(resolved.getBg().b == 0.9f);
        REQUIRE(resolved.getFg().r == 0.9f);
        REQUIRE(resolved.getFg().g == 0.1f);
        REQUIRE(resolved.getFg().b == 0.1f);
    }
}

TEST_CASE("FaceResolver preserves attributes", "[FaceResolver][resolve][Attributes]")
{
    domain::DefaultFace default_face(domain::NamedColor::Black, domain::NamedColor::White);
    domain::Face fallback_face(domain::NamedColor::Cyan, domain::NamedColor::Magenta,
                               {domain::Attribute::Dim, domain::Attribute::Underline});
    domain::ColorOverrides overrides;
    domain::FaceResolver resolver(fallback_face, default_face, overrides);

    SECTION("resolve preserves face attributes")
    {
        std::vector<domain::Attribute> face_attrs = {domain::Attribute::Bold, domain::Attribute::Italic};
        domain::Face face(domain::NamedColor::Red, domain::NamedColor::Blue, face_attrs);
        domain::ResolvedFace resolved = resolver.resolve(face);

        REQUIRE(resolved.hasAttribute(domain::Attribute::Bold));
        REQUIRE(resolved.hasAttribute(domain::Attribute::Italic));
        REQUIRE_FALSE(resolved.hasAttribute(domain::Attribute::Dim));
        REQUIRE_FALSE(resolved.hasAttribute(domain::Attribute::Underline));
    }

    SECTION("resolve preserves empty attributes")
    {
        domain::Face face(domain::NamedColor::Red, domain::NamedColor::Blue);
        domain::ResolvedFace resolved = resolver.resolve(face);

        REQUIRE_FALSE(resolved.hasAttribute(domain::Attribute::Bold));
        REQUIRE_FALSE(resolved.hasAttribute(domain::Attribute::Dim));
    }

    SECTION("resolve with DefaultColor preserves face attributes")
    {
        std::vector<domain::Attribute> face_attrs = {domain::Attribute::Bold, domain::Attribute::Blink};
        domain::Face face(domain::DefaultColor{}, domain::DefaultColor{}, face_attrs);
        domain::ResolvedFace resolved = resolver.resolve(face);

        REQUIRE(resolved.hasAttribute(domain::Attribute::Bold));
        REQUIRE(resolved.hasAttribute(domain::Attribute::Blink));
        REQUIRE_FALSE(resolved.hasAttribute(domain::Attribute::Dim));
        REQUIRE_FALSE(resolved.hasAttribute(domain::Attribute::Underline));
    }
}

TEST_CASE("FaceResolver const correctness", "[FaceResolver][const]")
{
    domain::DefaultFace default_face(domain::NamedColor::Black, domain::NamedColor::White);
    domain::Face fallback_face(domain::NamedColor::Red, domain::NamedColor::Blue);
    domain::ColorOverrides overrides;

    const domain::FaceResolver resolver(fallback_face, default_face, overrides);
    domain::Face face(domain::DefaultColor{}, domain::DefaultColor{});

    domain::ResolvedFace resolved = resolver.resolve(face);

    domain::RGBAColor expected_bg = domain::getRGBAColor(domain::NamedColor::Red);
    domain::RGBAColor expected_fg = domain::getRGBAColor(domain::NamedColor::Blue);

    REQUIRE(resolved.getBg().r == expected_bg.r);
    REQUIRE(resolved.getBg().g == expected_bg.g);
    REQUIRE(resolved.getBg().b == expected_bg.b);
    REQUIRE(resolved.getFg().r == expected_fg.r);
    REQUIRE(resolved.getFg().g == expected_fg.g);
    REQUIRE(resolved.getFg().b == expected_fg.b);
}

TEST_CASE("FaceResolver edge cases", "[FaceResolver][edge]")
{
    SECTION("empty color overrides")
    {
        domain::ColorOverrides empty_overrides;
        domain::DefaultFace default_face(domain::NamedColor::Black, domain::NamedColor::White);
        domain::Face face(domain::NamedColor::Red, domain::NamedColor::Blue);
        domain::FaceResolver resolver(std::nullopt, default_face, empty_overrides);

        domain::ResolvedFace resolved = resolver.resolve(face);

        domain::RGBAColor expected_bg = domain::getRGBAColor(domain::NamedColor::Red);
        domain::RGBAColor expected_fg = domain::getRGBAColor(domain::NamedColor::Blue);

        REQUIRE(resolved.getBg().r == expected_bg.r);
        REQUIRE(resolved.getBg().g == expected_bg.g);
        REQUIRE(resolved.getBg().b == expected_bg.b);
        REQUIRE(resolved.getFg().r == expected_fg.r);
        REQUIRE(resolved.getFg().g == expected_fg.g);
        REQUIRE(resolved.getFg().b == expected_fg.b);
    }

    SECTION("RGBAColor face with all default values")
    {
        domain::RGBAColor transparent{0.0f, 0.0f, 0.0f, 0.0f};
        domain::RGBAColor opaque_white{1.0f, 1.0f, 1.0f, 1.0f};
        domain::DefaultFace default_face(domain::NamedColor::Black, domain::NamedColor::White);
        domain::ColorOverrides overrides;
        domain::Face face(transparent, opaque_white);
        domain::FaceResolver resolver(std::nullopt, default_face, overrides);

        domain::ResolvedFace resolved = resolver.resolve(face);

        REQUIRE(resolved.getBg().r == 0.0f);
        REQUIRE(resolved.getBg().g == 0.0f);
        REQUIRE(resolved.getBg().b == 0.0f);
        REQUIRE(resolved.getBg().a == 0.0f);
        REQUIRE(resolved.getFg().r == 1.0f);
        REQUIRE(resolved.getFg().g == 1.0f);
        REQUIRE(resolved.getFg().b == 1.0f);
        REQUIRE(resolved.getFg().a == 1.0f);
    }

    SECTION("nested DefaultColor resolution")
    {
        domain::DefaultFace default_face(domain::NamedColor::Black, domain::NamedColor::White);
        domain::Face fallback_face(domain::DefaultColor{}, domain::DefaultColor{});
        domain::Face face(domain::DefaultColor{}, domain::DefaultColor{});
        domain::ColorOverrides overrides;

        domain::FaceResolver resolver(fallback_face, default_face, overrides);
        domain::ResolvedFace resolved = resolver.resolve(face);

        domain::RGBAColor expected_bg = domain::getRGBAColor(domain::NamedColor::Black);
        domain::RGBAColor expected_fg = domain::getRGBAColor(domain::NamedColor::White);

        REQUIRE(resolved.getBg().r == expected_bg.r);
        REQUIRE(resolved.getBg().g == expected_bg.g);
        REQUIRE(resolved.getBg().b == expected_bg.b);
        REQUIRE(resolved.getFg().r == expected_fg.r);
        REQUIRE(resolved.getFg().g == expected_fg.g);
        REQUIRE(resolved.getFg().b == expected_fg.b);
    }
}