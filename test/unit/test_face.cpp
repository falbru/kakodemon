#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "domain/color.hpp"
#include "domain/face.hpp"

TEST_CASE("DefaultFace constructor and getters", "[Face][DefaultFace]")
{
    SECTION("constructor with colors only")
    {
        domain::DefaultFace face(domain::NamedColor::Red, domain::NamedColor::Blue);

        domain::ColorOverrides no_overrides;
        domain::RGBAColor resolved_bg = face.resolveBg(no_overrides);
        domain::RGBAColor resolved_fg = face.resolveFg(no_overrides);

        REQUIRE(resolved_bg.r == 1.0f);
        REQUIRE(resolved_bg.g == 0.0f);
        REQUIRE(resolved_bg.b == 0.0f);
        REQUIRE(resolved_bg.a == 1.0f);

        REQUIRE(resolved_fg.r == 0.0f);
        REQUIRE(resolved_fg.g == 0.0f);
        REQUIRE(resolved_fg.b == 1.0f);
        REQUIRE(resolved_fg.a == 1.0f);
    }

    SECTION("constructor with colors and attributes")
    {
        std::vector<domain::Attribute> attributes = {domain::Attribute::Bold, domain::Attribute::Italic};
        domain::DefaultFace face(domain::DefaultColor{}, domain::DefaultColor{}, attributes);

        const auto &face_attributes = face.getAttributes();
        REQUIRE(face_attributes.size() == 2);
        REQUIRE(face.hasAttribute(domain::Attribute::Bold));
        REQUIRE(face.hasAttribute(domain::Attribute::Italic));
        REQUIRE_FALSE(face.hasAttribute(domain::Attribute::Underline));
    }

    SECTION("resolveBg with DefaultColor uses getDefaultBg")
    {
        domain::DefaultFace face(domain::DefaultColor{}, domain::NamedColor::Red);
        domain::ColorOverrides no_overrides;
        domain::RGBAColor resolved_bg = face.resolveBg(no_overrides);
        domain::RGBAColor expected = domain::getDefaultBg();

        REQUIRE(resolved_bg.r == expected.r);
        REQUIRE(resolved_bg.g == expected.g);
        REQUIRE(resolved_bg.b == expected.b);
        REQUIRE(resolved_bg.a == expected.a);
    }

    SECTION("resolveFg with DefaultColor uses getDefaultFg")
    {
        domain::DefaultFace face(domain::NamedColor::Blue, domain::DefaultColor{});
        domain::ColorOverrides no_overrides;
        domain::RGBAColor resolved_fg = face.resolveFg(no_overrides);
        domain::RGBAColor expected = domain::getDefaultFg();

        REQUIRE(resolved_fg.r == expected.r);
        REQUIRE(resolved_fg.g == expected.g);
        REQUIRE(resolved_fg.b == expected.b);
        REQUIRE(resolved_fg.a == expected.a);
    }

    SECTION("resolve with RGBAColor directly")
    {
        domain::RGBAColor custom_bg{0.5f, 0.5f, 0.5f, 1.0f};
        domain::RGBAColor custom_fg{0.8f, 0.2f, 0.2f, 1.0f};
        domain::DefaultFace face(custom_bg, custom_fg);
        domain::ColorOverrides no_overrides;

        domain::RGBAColor resolved_bg = face.resolveBg(no_overrides);
        domain::RGBAColor resolved_fg = face.resolveFg(no_overrides);

        REQUIRE(resolved_bg.r == 0.5f);
        REQUIRE(resolved_bg.g == 0.5f);
        REQUIRE(resolved_bg.b == 0.5f);
        REQUIRE(resolved_fg.r == 0.8f);
        REQUIRE(resolved_fg.g == 0.2f);
        REQUIRE(resolved_fg.b == 0.2f);
    }

    SECTION("resolve with NamedColor and overrides")
    {
        domain::ColorOverrides overrides;
        overrides[domain::NamedColor::Blue] = domain::RGBAColor{0.1f, 0.2f, 0.3f, 1.0f};

        domain::DefaultFace face(domain::NamedColor::Red, domain::NamedColor::Blue);
        domain::RGBAColor resolved_fg = face.resolveFg(overrides);

        REQUIRE(resolved_fg.r == 0.1f);
        REQUIRE(resolved_fg.g == 0.2f);
        REQUIRE(resolved_fg.b == 0.3f);
        REQUIRE(resolved_fg.a == 1.0f);
    }
}

TEST_CASE("DefaultFace equality operator", "[Face][DefaultFace]")
{
    SECTION("equal faces")
    {
        domain::DefaultFace face1(domain::NamedColor::Red, domain::NamedColor::Blue);
        domain::DefaultFace face2(domain::NamedColor::Red, domain::NamedColor::Blue);
        REQUIRE(face1 == face2);
    }

    SECTION("different colors")
    {
        domain::DefaultFace face1(domain::NamedColor::Red, domain::NamedColor::Blue);
        domain::DefaultFace face2(domain::NamedColor::Blue, domain::NamedColor::Red);
        REQUIRE_FALSE(face1 == face2);
    }

    SECTION("different attributes")
    {
        domain::DefaultFace face1(domain::DefaultColor{}, domain::DefaultColor{}, {domain::Attribute::Bold});
        domain::DefaultFace face2(domain::DefaultColor{}, domain::DefaultColor{}, {domain::Attribute::Italic});
        REQUIRE_FALSE(face1 == face2);
    }

    SECTION("equal with attributes")
    {
        domain::DefaultFace face1(domain::NamedColor::Red, domain::NamedColor::Blue,
                                  {domain::Attribute::Bold, domain::Attribute::Italic});
        domain::DefaultFace face2(domain::NamedColor::Red, domain::NamedColor::Blue,
                                  {domain::Attribute::Bold, domain::Attribute::Italic});
        REQUIRE(face1 == face2);
    }
}

TEST_CASE("ResolvedFace constructor and getters", "[Face][ResolvedFace]")
{
    SECTION("constructor with colors only")
    {
        domain::RGBAColor bg{0.1f, 0.2f, 0.3f, 1.0f};
        domain::RGBAColor fg{0.4f, 0.5f, 0.6f, 1.0f};
        domain::ResolvedFace face(bg, fg);

        REQUIRE(face.getBg().r == 0.1f);
        REQUIRE(face.getBg().g == 0.2f);
        REQUIRE(face.getBg().b == 0.3f);
        REQUIRE(face.getFg().r == 0.4f);
        REQUIRE(face.getFg().g == 0.5f);
        REQUIRE(face.getFg().b == 0.6f);
    }

    SECTION("constructor with colors and attributes")
    {
        domain::RGBAColor bg{1.0f, 1.0f, 1.0f, 1.0f};
        domain::RGBAColor fg{0.0f, 0.0f, 0.0f, 1.0f};
        std::vector<domain::Attribute> attributes = {domain::Attribute::Bold, domain::Attribute::Reverse};
        domain::ResolvedFace face(bg, fg, attributes);

        const auto &face_attributes = face.getAttributes();
        REQUIRE(face_attributes.size() == 2);
        REQUIRE(face.hasAttribute(domain::Attribute::Bold));
        REQUIRE(face.hasAttribute(domain::Attribute::Reverse));
        REQUIRE_FALSE(face.hasAttribute(domain::Attribute::Italic));
    }

    SECTION("getAttributes const correctness")
    {
        domain::RGBAColor bg{1.0f, 0.0f, 0.0f, 1.0f};
        domain::RGBAColor fg{0.0f, 1.0f, 0.0f, 1.0f};
        const domain::ResolvedFace face(bg, fg, {domain::Attribute::Underline});

        const auto &attributes = face.getAttributes();
        REQUIRE(attributes.size() == 1);
        REQUIRE(attributes[0] == domain::Attribute::Underline);
    }
}

TEST_CASE("ResolvedFace equality operator", "[Face][ResolvedFace]")
{
    SECTION("equal faces")
    {
        domain::RGBAColor bg{0.1f, 0.2f, 0.3f, 1.0f};
        domain::RGBAColor fg{0.4f, 0.5f, 0.6f, 1.0f};
        domain::ResolvedFace face1(bg, fg);
        domain::ResolvedFace face2(bg, fg);
        REQUIRE(face1 == face2);
    }

    SECTION("different colors")
    {
        domain::RGBAColor bg1{0.1f, 0.2f, 0.3f, 1.0f};
        domain::RGBAColor bg2{0.2f, 0.3f, 0.4f, 1.0f};
        domain::RGBAColor fg{0.4f, 0.5f, 0.6f, 1.0f};
        domain::ResolvedFace face1(bg1, fg);
        domain::ResolvedFace face2(bg2, fg);
        REQUIRE_FALSE(face1 == face2);
    }

    SECTION("different attributes")
    {
        domain::RGBAColor bg{1.0f, 1.0f, 1.0f, 1.0f};
        domain::RGBAColor fg{0.0f, 0.0f, 0.0f, 1.0f};
        domain::ResolvedFace face1(bg, fg, {domain::Attribute::Bold});
        domain::ResolvedFace face2(bg, fg, {domain::Attribute::Italic});
        REQUIRE_FALSE(face1 == face2);
    }
}

TEST_CASE("Face constructor and getters", "[Face][Face]")
{
    SECTION("constructor with colors only")
    {
        domain::Face face(domain::NamedColor::Red, domain::NamedColor::Blue);
        const auto &attributes = face.getAttributes();
        REQUIRE(attributes.empty());
        REQUIRE_FALSE(face.hasAttribute(domain::Attribute::Bold));
    }

    SECTION("constructor with colors and attributes")
    {
        std::vector<domain::Attribute> attributes = {domain::Attribute::Bold, domain::Attribute::Dim};
        domain::Face face(domain::DefaultColor{}, domain::NamedColor::Green, attributes);

        const auto &face_attributes = face.getAttributes();
        REQUIRE(face_attributes.size() == 2);
        REQUIRE(face.hasAttribute(domain::Attribute::Bold));
        REQUIRE(face.hasAttribute(domain::Attribute::Dim));
    }

    SECTION("getAttributes const correctness")
    {
        std::vector<domain::Attribute> attributes = {domain::Attribute::Underline};
        const domain::Face face(domain::DefaultColor{}, domain::DefaultColor{}, attributes);

        const auto &face_attributes = face.getAttributes();
        REQUIRE(face_attributes.size() == 1);
        REQUIRE(face_attributes[0] == domain::Attribute::Underline);
    }
}

TEST_CASE("Face equality operator", "[Face][Face]")
{
    SECTION("equal faces")
    {
        domain::Face face1(domain::NamedColor::Red, domain::NamedColor::Blue);
        domain::Face face2(domain::NamedColor::Red, domain::NamedColor::Blue);
        REQUIRE(face1 == face2);
    }

    SECTION("different colors")
    {
        domain::Face face1(domain::NamedColor::Red, domain::NamedColor::Blue);
        domain::Face face2(domain::NamedColor::Blue, domain::NamedColor::Red);
        REQUIRE_FALSE(face1 == face2);
    }

    SECTION("different attributes")
    {
        domain::Face face1(domain::DefaultColor{}, domain::DefaultColor{}, {domain::Attribute::Bold});
        domain::Face face2(domain::DefaultColor{}, domain::DefaultColor{}, {domain::Attribute::Italic});
        REQUIRE_FALSE(face1 == face2);
    }
}

TEST_CASE("Face resolveBg with DefaultFace", "[Face][resolveBg]")
{
    domain::DefaultFace default_face(domain::NamedColor::Green, domain::NamedColor::Yellow);
    domain::ColorOverrides no_overrides;

    SECTION("DefaultColor bg uses default_face bg")
    {
        domain::Face face(domain::DefaultColor{}, domain::NamedColor::Red);
        domain::RGBAColor resolved = face.resolveBg(default_face, no_overrides);
        domain::RGBAColor expected = default_face.resolveBg(no_overrides);

        REQUIRE(resolved.r == expected.r);
        REQUIRE(resolved.g == expected.g);
        REQUIRE(resolved.b == expected.b);
    }

    SECTION("NamedColor bg uses color directly")
    {
        domain::Face face(domain::NamedColor::Blue, domain::NamedColor::Red);
        domain::RGBAColor resolved = face.resolveBg(default_face, no_overrides);
        domain::RGBAColor expected = domain::getRGBAColor(domain::NamedColor::Blue);

        REQUIRE(resolved.r == expected.r);
        REQUIRE(resolved.g == expected.g);
        REQUIRE(resolved.b == expected.b);
    }

    SECTION("RGBAColor bg uses color directly")
    {
        domain::RGBAColor custom_bg{0.3f, 0.4f, 0.5f, 1.0f};
        domain::Face face(custom_bg, domain::NamedColor::Red);
        domain::RGBAColor resolved = face.resolveBg(default_face, no_overrides);

        REQUIRE(resolved.r == 0.3f);
        REQUIRE(resolved.g == 0.4f);
        REQUIRE(resolved.b == 0.5f);
    }
}

TEST_CASE("Face resolveFg with DefaultFace", "[Face][resolveFg]")
{
    domain::DefaultFace default_face(domain::NamedColor::Green, domain::NamedColor::Yellow);
    domain::ColorOverrides no_overrides;

    SECTION("DefaultColor fg uses default_face fg")
    {
        domain::Face face(domain::NamedColor::Red, domain::DefaultColor{});
        domain::RGBAColor resolved = face.resolveFg(default_face, no_overrides);
        domain::RGBAColor expected = default_face.resolveFg(no_overrides);

        REQUIRE(resolved.r == expected.r);
        REQUIRE(resolved.g == expected.g);
        REQUIRE(resolved.b == expected.b);
    }

    SECTION("NamedColor fg uses color directly")
    {
        domain::Face face(domain::NamedColor::Red, domain::NamedColor::Blue);
        domain::RGBAColor resolved = face.resolveFg(default_face, no_overrides);
        domain::RGBAColor expected = domain::getRGBAColor(domain::NamedColor::Blue);

        REQUIRE(resolved.r == expected.r);
        REQUIRE(resolved.g == expected.g);
        REQUIRE(resolved.b == expected.b);
    }

    SECTION("RGBAColor fg uses color directly")
    {
        domain::RGBAColor custom_fg{0.6f, 0.7f, 0.8f, 1.0f};
        domain::Face face(domain::NamedColor::Red, custom_fg);
        domain::RGBAColor resolved = face.resolveFg(default_face, no_overrides);

        REQUIRE(resolved.r == 0.6f);
        REQUIRE(resolved.g == 0.7f);
        REQUIRE(resolved.b == 0.8f);
    }
}

TEST_CASE("Face resolve with DefaultFace", "[Face][resolve]")
{
    domain::DefaultFace default_face(domain::NamedColor::Green, domain::NamedColor::Yellow);
    domain::ColorOverrides no_overrides;

    SECTION("resolve returns ResolvedFace with correct colors and attributes")
    {
        std::vector<domain::Attribute> attributes = {domain::Attribute::Bold, domain::Attribute::Italic};
        domain::Face face(domain::NamedColor::Red, domain::NamedColor::Blue, attributes);
        domain::ResolvedFace resolved = face.resolve(default_face, no_overrides);

        domain::RGBAColor expected_bg = domain::getRGBAColor(domain::NamedColor::Red);
        domain::RGBAColor expected_fg = domain::getRGBAColor(domain::NamedColor::Blue);

        REQUIRE(resolved.getBg().r == expected_bg.r);
        REQUIRE(resolved.getBg().g == expected_bg.g);
        REQUIRE(resolved.getBg().b == expected_bg.b);
        REQUIRE(resolved.getFg().r == expected_fg.r);
        REQUIRE(resolved.getFg().g == expected_fg.g);
        REQUIRE(resolved.getFg().b == expected_fg.b);

        const auto &resolved_attributes = resolved.getAttributes();
        REQUIRE(resolved_attributes.size() == 2);
        REQUIRE(resolved.hasAttribute(domain::Attribute::Bold));
        REQUIRE(resolved.hasAttribute(domain::Attribute::Italic));
    }

    SECTION("resolve with DefaultColor uses default_face colors")
    {
        domain::Face face(domain::DefaultColor{}, domain::DefaultColor{}, {domain::Attribute::Underline});
        domain::ResolvedFace resolved = face.resolve(default_face, no_overrides);

        domain::RGBAColor expected_bg = default_face.resolveBg(no_overrides);
        domain::RGBAColor expected_fg = default_face.resolveFg(no_overrides);

        REQUIRE(resolved.getBg().r == expected_bg.r);
        REQUIRE(resolved.getBg().g == expected_bg.g);
        REQUIRE(resolved.getBg().b == expected_bg.b);
        REQUIRE(resolved.getFg().r == expected_fg.r);
        REQUIRE(resolved.getFg().g == expected_fg.g);
        REQUIRE(resolved.getFg().b == expected_fg.b);

        REQUIRE(resolved.hasAttribute(domain::Attribute::Underline));
    }
}

TEST_CASE("Face resolve with fallback face", "[Face][resolve]")
{
    domain::DefaultFace default_face(domain::NamedColor::Black, domain::NamedColor::White);
    domain::Face fallback_face(domain::NamedColor::Cyan, domain::NamedColor::Magenta, {domain::Attribute::Dim});
    domain::ColorOverrides no_overrides;

    SECTION("resolve uses fallback for DefaultColor bg")
    {
        domain::Face face(domain::DefaultColor{}, domain::NamedColor::Red);
        domain::ResolvedFace resolved = face.resolve(fallback_face, default_face, no_overrides);

        domain::RGBAColor expected_bg = fallback_face.resolveBg(default_face, no_overrides);
        domain::RGBAColor expected_fg = domain::getRGBAColor(domain::NamedColor::Red);

        REQUIRE(resolved.getBg().r == expected_bg.r);
        REQUIRE(resolved.getBg().g == expected_bg.g);
        REQUIRE(resolved.getBg().b == expected_bg.b);
        REQUIRE(resolved.getFg().r == expected_fg.r);
        REQUIRE(resolved.getFg().g == expected_fg.g);
        REQUIRE(resolved.getFg().b == expected_fg.b);
    }

    SECTION("resolve uses fallback for DefaultColor fg")
    {
        domain::Face face(domain::NamedColor::Blue, domain::DefaultColor{});
        domain::ResolvedFace resolved = face.resolve(fallback_face, default_face, no_overrides);

        domain::RGBAColor expected_bg = domain::getRGBAColor(domain::NamedColor::Blue);
        domain::RGBAColor expected_fg = fallback_face.resolveFg(default_face, no_overrides);

        REQUIRE(resolved.getBg().r == expected_bg.r);
        REQUIRE(resolved.getBg().g == expected_bg.g);
        REQUIRE(resolved.getBg().b == expected_bg.b);
        REQUIRE(resolved.getFg().r == expected_fg.r);
        REQUIRE(resolved.getFg().g == expected_fg.g);
        REQUIRE(resolved.getFg().b == expected_fg.b);
    }

    SECTION("resolve preserves own attributes, not fallback")
    {
        std::vector<domain::Attribute> face_attributes = {domain::Attribute::Bold};
        domain::Face face(domain::NamedColor::Red, domain::NamedColor::Blue, face_attributes);
        domain::ResolvedFace resolved = face.resolve(fallback_face, default_face, no_overrides);

        REQUIRE(resolved.hasAttribute(domain::Attribute::Bold));
        REQUIRE_FALSE(resolved.hasAttribute(domain::Attribute::Dim));
    }

    SECTION("resolve uses default_face when fallback also has DefaultColor")
    {
        domain::Face fallback_with_default(domain::DefaultColor{}, domain::NamedColor::Red);
        domain::Face face(domain::DefaultColor{}, domain::NamedColor::Blue);
        domain::ResolvedFace resolved = face.resolve(fallback_with_default, default_face, no_overrides);

        domain::RGBAColor expected_bg = default_face.resolveBg(no_overrides);
        domain::RGBAColor expected_fg = domain::getRGBAColor(domain::NamedColor::Blue);

        REQUIRE(resolved.getBg().r == expected_bg.r);
        REQUIRE(resolved.getBg().g == expected_bg.g);
        REQUIRE(resolved.getBg().b == expected_bg.b);
        REQUIRE(resolved.getFg().r == expected_fg.r);
        REQUIRE(resolved.getFg().g == expected_fg.g);
        REQUIRE(resolved.getFg().b == expected_fg.b);
    }
}

TEST_CASE("Face resolve with color overrides", "[Face][resolve][ColorOverrides]")
{
    domain::ColorOverrides overrides;
    overrides[domain::NamedColor::Red] = domain::RGBAColor{0.9f, 0.1f, 0.1f, 1.0f};
    overrides[domain::NamedColor::Blue] = domain::RGBAColor{0.1f, 0.1f, 0.9f, 1.0f};

    domain::DefaultFace default_face(domain::NamedColor::Black, domain::NamedColor::White);

    SECTION("resolve uses color overrides for NamedColor")
    {
        domain::Face face(domain::NamedColor::Red, domain::NamedColor::Blue);
        domain::ResolvedFace resolved = face.resolve(default_face, overrides);

        REQUIRE(resolved.getBg().r == 0.9f);
        REQUIRE(resolved.getBg().g == 0.1f);
        REQUIRE(resolved.getBg().b == 0.1f);
        REQUIRE(resolved.getFg().r == 0.1f);
        REQUIRE(resolved.getFg().g == 0.1f);
        REQUIRE(resolved.getFg().b == 0.9f);
    }

    SECTION("resolve ignores overrides for RGBAColor")
    {
        domain::RGBAColor custom_bg{0.2f, 0.3f, 0.4f, 1.0f};
        domain::RGBAColor custom_fg{0.5f, 0.6f, 0.7f, 1.0f};
        domain::Face face(custom_bg, custom_fg);
        domain::ResolvedFace resolved = face.resolve(default_face, overrides);

        REQUIRE(resolved.getBg().r == 0.2f);
        REQUIRE(resolved.getBg().g == 0.3f);
        REQUIRE(resolved.getBg().b == 0.4f);
        REQUIRE(resolved.getFg().r == 0.5f);
        REQUIRE(resolved.getFg().g == 0.6f);
        REQUIRE(resolved.getFg().b == 0.7f);
    }

    SECTION("resolve uses overrides in fallback resolution")
    {
        domain::Face fallback_face(domain::NamedColor::Red, domain::NamedColor::Blue);
        domain::Face face(domain::DefaultColor{}, domain::DefaultColor{});
        domain::ResolvedFace resolved = face.resolve(fallback_face, default_face, overrides);

        REQUIRE(resolved.getBg().r == 0.9f);
        REQUIRE(resolved.getBg().g == 0.1f);
        REQUIRE(resolved.getBg().b == 0.1f);
        REQUIRE(resolved.getFg().r == 0.1f);
        REQUIRE(resolved.getFg().g == 0.1f);
        REQUIRE(resolved.getFg().b == 0.9f);
    }
}

TEST_CASE("Face hasAttribute method", "[Face][hasAttribute]")
{
    SECTION("hasAttribute with empty attributes")
    {
        domain::Face face(domain::NamedColor::Red, domain::NamedColor::Blue);
        REQUIRE_FALSE(face.hasAttribute(domain::Attribute::Bold));
        REQUIRE_FALSE(face.hasAttribute(domain::Attribute::Italic));
    }

    SECTION("hasAttribute with populated attributes")
    {
        std::vector<domain::Attribute> attributes = {domain::Attribute::Bold, domain::Attribute::Underline};
        domain::Face face(domain::NamedColor::Red, domain::NamedColor::Blue, attributes);

        REQUIRE(face.hasAttribute(domain::Attribute::Bold));
        REQUIRE(face.hasAttribute(domain::Attribute::Underline));
        REQUIRE_FALSE(face.hasAttribute(domain::Attribute::Italic));
        REQUIRE_FALSE(face.hasAttribute(domain::Attribute::Reverse));
    }
}