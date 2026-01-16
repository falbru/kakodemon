#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "adapters/kakoune/color.hpp"
#include "domain/color.hpp"

TEST_CASE("toDomain converts default color", "[Color][toDomain]") {
    kakoune::Color color{"default"};
    domain::OptionalColor result = kakoune::toDomain(color);
    REQUIRE(std::holds_alternative<domain::DefaultColor>(result));
}

TEST_CASE("toDomain converts named colors", "[Color][toDomain]") {
    SECTION("black") {
        kakoune::Color color{"black"};
        domain::OptionalColor result = kakoune::toDomain(color);
        REQUIRE(std::holds_alternative<domain::FixedColor>(result));
        REQUIRE(std::get<domain::FixedColor>(result) == domain::FixedColor::Black);
    }

    SECTION("red") {
        kakoune::Color color{"red"};
        domain::OptionalColor result = kakoune::toDomain(color);
        REQUIRE(std::holds_alternative<domain::FixedColor>(result));
        REQUIRE(std::get<domain::FixedColor>(result) == domain::FixedColor::Red);
    }

    SECTION("green") {
        kakoune::Color color{"green"};
        domain::OptionalColor result = kakoune::toDomain(color);
        REQUIRE(std::holds_alternative<domain::FixedColor>(result));
        REQUIRE(std::get<domain::FixedColor>(result) == domain::FixedColor::Green);
    }

    SECTION("yellow") {
        kakoune::Color color{"yellow"};
        domain::OptionalColor result = kakoune::toDomain(color);
        REQUIRE(std::holds_alternative<domain::FixedColor>(result));
        REQUIRE(std::get<domain::FixedColor>(result) == domain::FixedColor::Yellow);
    }

    SECTION("blue") {
        kakoune::Color color{"blue"};
        domain::OptionalColor result = kakoune::toDomain(color);
        REQUIRE(std::holds_alternative<domain::FixedColor>(result));
        REQUIRE(std::get<domain::FixedColor>(result) == domain::FixedColor::Blue);
    }

    SECTION("magenta") {
        kakoune::Color color{"magenta"};
        domain::OptionalColor result = kakoune::toDomain(color);
        REQUIRE(std::holds_alternative<domain::FixedColor>(result));
        REQUIRE(std::get<domain::FixedColor>(result) == domain::FixedColor::Magenta);
    }

    SECTION("cyan") {
        kakoune::Color color{"cyan"};
        domain::OptionalColor result = kakoune::toDomain(color);
        REQUIRE(std::holds_alternative<domain::FixedColor>(result));
        REQUIRE(std::get<domain::FixedColor>(result) == domain::FixedColor::Cyan);
    }

    SECTION("white") {
        kakoune::Color color{"white"};
        domain::OptionalColor result = kakoune::toDomain(color);
        REQUIRE(std::holds_alternative<domain::FixedColor>(result));
        REQUIRE(std::get<domain::FixedColor>(result) == domain::FixedColor::White);
    }
}

TEST_CASE("toDomain converts bright named colors", "[Color][toDomain]") {
    SECTION("bright-black") {
        kakoune::Color color{"bright-black"};
        domain::OptionalColor result = kakoune::toDomain(color);
        REQUIRE(std::holds_alternative<domain::FixedColor>(result));
        REQUIRE(std::get<domain::FixedColor>(result) == domain::FixedColor::BrightBlack);
    }

    SECTION("bright-red") {
        kakoune::Color color{"bright-red"};
        domain::OptionalColor result = kakoune::toDomain(color);
        REQUIRE(std::holds_alternative<domain::FixedColor>(result));
        REQUIRE(std::get<domain::FixedColor>(result) == domain::FixedColor::BrightRed);
    }

    SECTION("bright-green") {
        kakoune::Color color{"bright-green"};
        domain::OptionalColor result = kakoune::toDomain(color);
        REQUIRE(std::holds_alternative<domain::FixedColor>(result));
        REQUIRE(std::get<domain::FixedColor>(result) == domain::FixedColor::BrightGreen);
    }

    SECTION("bright-yellow") {
        kakoune::Color color{"bright-yellow"};
        domain::OptionalColor result = kakoune::toDomain(color);
        REQUIRE(std::holds_alternative<domain::FixedColor>(result));
        REQUIRE(std::get<domain::FixedColor>(result) == domain::FixedColor::BrightYellow);
    }

    SECTION("bright-blue") {
        kakoune::Color color{"bright-blue"};
        domain::OptionalColor result = kakoune::toDomain(color);
        REQUIRE(std::holds_alternative<domain::FixedColor>(result));
        REQUIRE(std::get<domain::FixedColor>(result) == domain::FixedColor::BrightBlue);
    }

    SECTION("bright-magenta") {
        kakoune::Color color{"bright-magenta"};
        domain::OptionalColor result = kakoune::toDomain(color);
        REQUIRE(std::holds_alternative<domain::FixedColor>(result));
        REQUIRE(std::get<domain::FixedColor>(result) == domain::FixedColor::BrightMagenta);
    }

    SECTION("bright-cyan") {
        kakoune::Color color{"bright-cyan"};
        domain::OptionalColor result = kakoune::toDomain(color);
        REQUIRE(std::holds_alternative<domain::FixedColor>(result));
        REQUIRE(std::get<domain::FixedColor>(result) == domain::FixedColor::BrightCyan);
    }

    SECTION("bright-white") {
        kakoune::Color color{"bright-white"};
        domain::OptionalColor result = kakoune::toDomain(color);
        REQUIRE(std::holds_alternative<domain::FixedColor>(result));
        REQUIRE(std::get<domain::FixedColor>(result) == domain::FixedColor::BrightWhite);
    }
}

TEST_CASE("toDomain converts rgb colors", "[Color][toDomain]") {
    SECTION("rgb:000000 is black") {
        kakoune::Color color{"rgb:000000"};
        domain::OptionalColor result = kakoune::toDomain(color);
        REQUIRE(std::holds_alternative<domain::RGBAColor>(result));
        domain::RGBAColor rgba = std::get<domain::RGBAColor>(result);
        REQUIRE(rgba.r == 0.0f);
        REQUIRE(rgba.g == 0.0f);
        REQUIRE(rgba.b == 0.0f);
        REQUIRE(rgba.a == 1.0f);
    }

    SECTION("rgb:ffffff is white") {
        kakoune::Color color{"rgb:ffffff"};
        domain::OptionalColor result = kakoune::toDomain(color);
        REQUIRE(std::holds_alternative<domain::RGBAColor>(result));
        domain::RGBAColor rgba = std::get<domain::RGBAColor>(result);
        REQUIRE(rgba.r == 1.0f);
        REQUIRE(rgba.g == 1.0f);
        REQUIRE(rgba.b == 1.0f);
        REQUIRE(rgba.a == 1.0f);
    }

    SECTION("rgb:ff0000 is red") {
        kakoune::Color color{"rgb:ff0000"};
        domain::OptionalColor result = kakoune::toDomain(color);
        REQUIRE(std::holds_alternative<domain::RGBAColor>(result));
        domain::RGBAColor rgba = std::get<domain::RGBAColor>(result);
        REQUIRE(rgba.r == 1.0f);
        REQUIRE(rgba.g == 0.0f);
        REQUIRE(rgba.b == 0.0f);
        REQUIRE(rgba.a == 1.0f);
    }

    SECTION("rgb:00ff00 is green") {
        kakoune::Color color{"rgb:00ff00"};
        domain::OptionalColor result = kakoune::toDomain(color);
        REQUIRE(std::holds_alternative<domain::RGBAColor>(result));
        domain::RGBAColor rgba = std::get<domain::RGBAColor>(result);
        REQUIRE(rgba.r == 0.0f);
        REQUIRE(rgba.g == 1.0f);
        REQUIRE(rgba.b == 0.0f);
        REQUIRE(rgba.a == 1.0f);
    }

    SECTION("rgb:0000ff is blue") {
        kakoune::Color color{"rgb:0000ff"};
        domain::OptionalColor result = kakoune::toDomain(color);
        REQUIRE(std::holds_alternative<domain::RGBAColor>(result));
        domain::RGBAColor rgba = std::get<domain::RGBAColor>(result);
        REQUIRE(rgba.r == 0.0f);
        REQUIRE(rgba.g == 0.0f);
        REQUIRE(rgba.b == 1.0f);
        REQUIRE(rgba.a == 1.0f);
    }

    SECTION("rgb with uppercase hex") {
        kakoune::Color color{"rgb:AABBCC"};
        domain::OptionalColor result = kakoune::toDomain(color);
        REQUIRE(std::holds_alternative<domain::RGBAColor>(result));
        domain::RGBAColor rgba = std::get<domain::RGBAColor>(result);
        REQUIRE(rgba.r == Catch::Approx(170.0f / 255.0f));
        REQUIRE(rgba.g == Catch::Approx(187.0f / 255.0f));
        REQUIRE(rgba.b == Catch::Approx(204.0f / 255.0f));
        REQUIRE(rgba.a == 1.0f);
    }
}

TEST_CASE("toDomain converts rgba colors", "[Color][toDomain]") {
    SECTION("rgba:00000000 is transparent black") {
        kakoune::Color color{"rgba:00000000"};
        domain::OptionalColor result = kakoune::toDomain(color);
        REQUIRE(std::holds_alternative<domain::RGBAColor>(result));
        domain::RGBAColor rgba = std::get<domain::RGBAColor>(result);
        REQUIRE(rgba.r == 0.0f);
        REQUIRE(rgba.g == 0.0f);
        REQUIRE(rgba.b == 0.0f);
        REQUIRE(rgba.a == 0.0f);
    }

    SECTION("rgba:ffffffff is opaque white") {
        kakoune::Color color{"rgba:ffffffff"};
        domain::OptionalColor result = kakoune::toDomain(color);
        REQUIRE(std::holds_alternative<domain::RGBAColor>(result));
        domain::RGBAColor rgba = std::get<domain::RGBAColor>(result);
        REQUIRE(rgba.r == 1.0f);
        REQUIRE(rgba.g == 1.0f);
        REQUIRE(rgba.b == 1.0f);
        REQUIRE(rgba.a == 1.0f);
    }

    SECTION("rgba:ff000080 is semi-transparent red") {
        kakoune::Color color{"rgba:ff000080"};
        domain::OptionalColor result = kakoune::toDomain(color);
        REQUIRE(std::holds_alternative<domain::RGBAColor>(result));
        domain::RGBAColor rgba = std::get<domain::RGBAColor>(result);
        REQUIRE(rgba.r == 1.0f);
        REQUIRE(rgba.g == 0.0f);
        REQUIRE(rgba.b == 0.0f);
        REQUIRE(rgba.a == Catch::Approx(128.0f / 255.0f));
    }
}

TEST_CASE("toDomain throws ColorConversionException for invalid colors", "[Color][toDomain]") {
    SECTION("invalid color name") {
        kakoune::Color color{"invalidcolor"};
        REQUIRE_THROWS_AS(kakoune::toDomain(color), kakoune::ColorConversionException);
    }

    SECTION("empty string") {
        kakoune::Color color{""};
        REQUIRE_THROWS_AS(kakoune::toDomain(color), kakoune::ColorConversionException);
    }

    SECTION("rgb with invalid hex") {
        kakoune::Color color{"rgb:gggggg"};
        REQUIRE_THROWS_AS(kakoune::toDomain(color), kakoune::ColorConversionException);
    }

    SECTION("rgb with wrong length") {
        kakoune::Color color{"rgb:fff"};
        REQUIRE_THROWS_AS(kakoune::toDomain(color), kakoune::ColorConversionException);
    }

    SECTION("rgba with invalid hex") {
        kakoune::Color color{"rgba:gggggggg"};
        REQUIRE_THROWS_AS(kakoune::toDomain(color), kakoune::ColorConversionException);
    }

    SECTION("rgba with wrong length") {
        kakoune::Color color{"rgba:ffffff"};
        REQUIRE_THROWS_AS(kakoune::toDomain(color), kakoune::ColorConversionException);
    }
}

TEST_CASE("ColorConversionException contains color string in message", "[Color][ColorConversionException]") {
    kakoune::Color color{"badcolor"};
    try {
        kakoune::toDomain(color);
        FAIL("Expected ColorConversionException to be thrown");
    } catch (const kakoune::ColorConversionException& e) {
        std::string what = e.what();
        REQUIRE(what.find("badcolor") != std::string::npos);
    }
}
