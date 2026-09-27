#include <catch2/catch_test_macros.hpp>

#include "domain/span.hpp"

TEST_CASE("Span constructor", "[Span]")
{
    domain::Span<int> span(1, 2);

    REQUIRE(span.value == 1);
    REQUIRE(span.start_index == 2);
}

TEST_CASE("Span comparator", "[Span]")
{
    domain::Span<int> span1(1, 2);
    domain::Span<int> span2(10, 0);

    REQUIRE(span2 < span1);
}

TEST_CASE("spanIteratorFromIndex", "[Span]")
{
    std::vector<domain::Span<int>> spans({
        domain::Span<int>(0, 0),
        domain::Span<int>(1, 3),
        domain::Span<int>(2, 5),
        domain::Span<int>(3, 6),
        domain::Span<int>(4, 10),
        domain::Span<int>(5, 20),
        domain::Span<int>(6, 25),
        domain::Span<int>(7, 30),
    });

    REQUIRE(spanIteratorFromIndex(spans, 0).base() == &spans[0]);
    REQUIRE(spanIteratorFromIndex(spans, 2).base() == &spans[0]);
    REQUIRE(spanIteratorFromIndex(spans, 5).base() == &spans[2]);
    REQUIRE(spanIteratorFromIndex(spans, 5).base() == &spans[2]);
    REQUIRE(spanIteratorFromIndex(spans, 10).base() == &spans[4]);
    REQUIRE(spanIteratorFromIndex(spans, 21).base() == &spans[5]);
    REQUIRE(spanIteratorFromIndex(spans, 29).base() == &spans[6]);
    REQUIRE(spanIteratorFromIndex(spans, 32).base() == &spans[7]);
}
