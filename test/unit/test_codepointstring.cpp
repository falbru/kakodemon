#include <catch2/catch_test_macros.hpp>

#include "domain/codepointstring.hpp"

TEST_CASE("CodepointString handles empty string", "[CodepointString]") {
    domain::CodepointString empty("");
    REQUIRE(empty.toString() == "");
    REQUIRE(empty.size() == 0);
}

TEST_CASE("CodepointString handles basic ASCII string", "[CodepointString]") {
    domain::CodepointString empty("SENTENCE");
    REQUIRE(empty.toString() == "SENTENCE");
    REQUIRE(empty.size() == 8);
}

TEST_CASE("CodepointString handles multi-byte UTF-8 characters", "[CodepointString]") {
    domain::CodepointString unicode("Hello 世界 🌍");
    REQUIRE(unicode.toString() == "Hello 世界 🌍");
}

TEST_CASE("CodepointString size counts codepoints not bytes", "[CodepointString][size]") {
    domain::CodepointString ascii("ABC");
    domain::CodepointString unicode("世界");
    REQUIRE(ascii.size() == 3);
    REQUIRE(unicode.size() == 2);
}

TEST_CASE("CodepointString at accesses character by index", "[CodepointString][at]") {
    domain::CodepointString ascii("ABC");

    REQUIRE(ascii.at(0) == 'A');
    REQUIRE(ascii.at(1) == 'B');
    REQUIRE(ascii.at(2) == 'C');
}

TEST_CASE("CodepointString addCodepoint appends to end", "[CodepointString][addCodepoint]") {
    domain::CodepointString ascii("ABC");

    REQUIRE(ascii.size() == 3);

    ascii.addCodepoint('D');

    REQUIRE(ascii.size() == 4);
    REQUIRE(ascii.at(3) == 'D');
}

TEST_CASE("CodepointString insertCodepoint adds at position", "[CodepointString][insertCodepoint]") {
    domain::CodepointString ascii("ABC");

    REQUIRE(ascii.size() == 3);

    ascii.insertCodepoint(0, 'D');

    REQUIRE(ascii.size() == 4);
    REQUIRE(ascii.at(0) == 'D');
}

TEST_CASE("CodepointString substring extracts from start to end", "[CodepointString][substring]") {
    domain::CodepointString ascii("ABCDEF");

    domain::CodepointString substr = ascii.substring(2);

    REQUIRE(substr.size() == 4);
    REQUIRE(substr.toString() == "CDEF");

    substr = ascii.substring(3);

    REQUIRE(substr.size() == 3);
    REQUIRE(substr.toString() == "DEF");
}

TEST_CASE("CodepointString substring at boundary returns empty", "[CodepointString][substring]") {
    domain::CodepointString ascii("ABC");

    domain::CodepointString substr = ascii.substring(3);

    REQUIRE(substr.size() == 0);
}

TEST_CASE("CodepointString substring with zero length returns empty", "[CodepointString][substring]") {
    domain::CodepointString ascii("ABCDEF");

    domain::CodepointString substr = ascii.substring(3, 0);

    REQUIRE(substr.size() == 0);
}

TEST_CASE("CodepointString substring with start and length", "[CodepointString][substring]") {
    domain::CodepointString ascii("ABCDEF");

    domain::CodepointString substr = ascii.substring(3, 1);

    REQUIRE(substr.size() == 1);
    REQUIRE(substr.toString() == "D");

    substr = ascii.substring(3, 2);

    REQUIRE(substr.size() == 2);
    REQUIRE(substr.toString() == "DE");

    substr = ascii.substring(3, 3);

    REQUIRE(substr.size() == 3);
    REQUIRE(substr.toString() == "DEF");
}

TEST_CASE("CodepointString trim removes whitespace from both ends", "[CodepointString][trim]") {
    domain::CodepointString ascii("     ABC     ");

    domain::CodepointString trim = ascii.trim(domain::TrimDirection::Both);

    REQUIRE(trim.size() == 3);
    REQUIRE(trim.toString() == "ABC");
}

TEST_CASE("CodepointString trim removes whitespace from right only", "[CodepointString][trim]") {
    domain::CodepointString ascii("     ABC     ");

    domain::CodepointString trim = ascii.trim(domain::TrimDirection::Right);

    REQUIRE(trim.size() == 8);
    REQUIRE(trim.toString() == "     ABC");
}

TEST_CASE("CodepointString trim removes whitespace from left only", "[CodepointString][trim]") {
    domain::CodepointString ascii("     ABC     ");

    domain::CodepointString trim = ascii.trim(domain::TrimDirection::Left);

    REQUIRE(trim.size() == 8);
    REQUIRE(trim.toString() == "ABC     ");
}

TEST_CASE("CodepointString converts to std::string correctly", "[CodepointString][toString]") {
    domain::CodepointString test("THIS IS A TEST");

    REQUIRE(test.toString() == "THIS IS A TEST");
}

TEST_CASE("CodepointString concatenates with += operator", "[CodepointString][operator]") {
    domain::CodepointString test;
    test += domain::CodepointString("THIS ");
    test += domain::CodepointString("IS ");
    test += domain::CodepointString("A ");
    test += domain::CodepointString("TEST");

    REQUIRE(test.toString() == "THIS IS A TEST");
}

TEST_CASE("CodepointString compares equal with == operator", "[CodepointString][operator]") {
    domain::CodepointString test1("THIS IS A TEST");
    domain::CodepointString test2("THIS IS A TEST");

    REQUIRE(test1 == test2);
    REQUIRE(test1.toString() == test2.toString());
    REQUIRE(test1.size() == test2.size());

    for (int i = 0; i < test1.size(); i++) {
        REQUIRE(test1.at(i) == test2.at(i));
    }
}

TEST_CASE("CodepointString compares unequal strings correctly with != operator", "[CodepointString][operator]") {
    domain::CodepointString test1("THIS IS A TEST");
    domain::CodepointString test2("THIS IS DIFFERENT");
    REQUIRE(test1 != test2);
}
