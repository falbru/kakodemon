#include "domain/geometry.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Empty rectangle constructor", "[rectangle]") {
    domain::Rectangle rectangle;

    REQUIRE(rectangle.left() == 0);
    REQUIRE(rectangle.top() == 0);
    REQUIRE(rectangle.width() == 0);
    REQUIRE(rectangle.height() == 0);
}

TEST_CASE("Rectangle constructor", "[rectangle]") {
    domain::Rectangle rectangle(2, 5, 10, 13);

    REQUIRE(rectangle.left() == 2);
    REQUIRE(rectangle.top() == 5);
    REQUIRE(rectangle.right() == 12);
    REQUIRE(rectangle.bottom() == 18);
    REQUIRE(rectangle.width() == 10);
    REQUIRE(rectangle.height() == 13);
}

TEST_CASE("Rectangle intersection", "[rectangle]") {
    SECTION("non-overlapping rectangles") {
        domain::Rectangle a(0, 0, 5, 5);
        domain::Rectangle b(10, 10, 5, 5);

        REQUIRE(!a.intersects(b));
        REQUIRE(!a.intersection(b).has_value());
    }

    SECTION("overlapping rectangles") {
        domain::Rectangle a(0, 0, 5, 5);
        domain::Rectangle b(2, 2, 5, 5);

        REQUIRE(a.intersects(b));

        auto intersection = a.intersection(b);

        REQUIRE(intersection.has_value());
        REQUIRE(intersection->left() == 2);
        REQUIRE(intersection->top() == 2);
        REQUIRE(intersection->width() == 3);
        REQUIRE(intersection->height() == 3);
    }

    SECTION("zero-width rectangle") {
        domain::Rectangle a(0, 0, 5, 5);
        domain::Rectangle b(2, 2, 0, 5);

        REQUIRE(!a.intersects(b));
        REQUIRE(!a.intersection(b).has_value());
    }
}
