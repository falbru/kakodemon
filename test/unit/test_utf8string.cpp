#include <catch2/catch_test_macros.hpp>

#include "domain/utf8string.hpp"

TEST_CASE("UTF8String handles empty string", "[UTF8String]") {
    domain::UTF8String empty("");
    REQUIRE(empty.toString() == "");
    REQUIRE(empty.size() == 0);
}

TEST_CASE("UTF8String handles basic ASCII string", "[UTF8String]") {
    domain::UTF8String empty("SENTENCE");
    REQUIRE(empty.toString() == "SENTENCE");
    REQUIRE(empty.size() == 8);
}

TEST_CASE("UTF8String handles multi-byte UTF-8 characters", "[UTF8String]") {
    domain::UTF8String unicode("Hello 世界 🌍");
    REQUIRE(unicode.toString() == "Hello 世界 🌍");
}

TEST_CASE("UTF8String size counts codepoints not bytes", "[UTF8String][size]") {
    domain::UTF8String ascii("ABC");
    domain::UTF8String unicode("世界");
    REQUIRE(ascii.size() == 3);
    REQUIRE(unicode.size() == 2);
}

TEST_CASE("UTF8String at accesses character by index", "[UTF8String][at]") {
    domain::UTF8String ascii("ABC");

    REQUIRE(ascii.at(0) == 'A');
    REQUIRE(ascii.at(1) == 'B');
    REQUIRE(ascii.at(2) == 'C');
}

TEST_CASE("UTF8String addCodepoint appends to end", "[UTF8String][addCodepoint]") {
    domain::UTF8String ascii("ABC");

    REQUIRE(ascii.size() == 3);

    ascii.addCodepoint('D');

    REQUIRE(ascii.size() == 4);
    REQUIRE(ascii.at(3) == 'D');
}

TEST_CASE("UTF8String insertCodepoint adds at position", "[UTF8String][insertCodepoint]") {
    domain::UTF8String ascii("ABC");

    REQUIRE(ascii.size() == 3);

    ascii.insertCodepoint(0, 'D');

    REQUIRE(ascii.size() == 4);
    REQUIRE(ascii.at(0) == 'D');
}

TEST_CASE("UTF8String substring extracts from start to end", "[UTF8String][substring]") {
    domain::UTF8String ascii("ABCDEF");

    domain::UTF8String substr = ascii.substring(2);

    REQUIRE(substr.size() == 4);
    REQUIRE(substr.toString() == "CDEF");

    substr = ascii.substring(3);

    REQUIRE(substr.size() == 3);
    REQUIRE(substr.toString() == "DEF");
}

TEST_CASE("UTF8String substring at boundary returns empty", "[UTF8String][substring]") {
    domain::UTF8String ascii("ABC");

    domain::UTF8String substr = ascii.substring(3);

    REQUIRE(substr.size() == 0);
}

TEST_CASE("UTF8String substring with zero length returns empty", "[UTF8String][substring]") {
    domain::UTF8String ascii("ABCDEF");

    domain::UTF8String substr = ascii.substring(3, 0);

    REQUIRE(substr.size() == 0);
}

TEST_CASE("UTF8String substring with start and length", "[UTF8String][substring]") {
    domain::UTF8String ascii("ABCDEF");

    domain::UTF8String substr = ascii.substring(3, 1);

    REQUIRE(substr.size() == 1);
    REQUIRE(substr.toString() == "D");

    substr = ascii.substring(3, 2);

    REQUIRE(substr.size() == 2);
    REQUIRE(substr.toString() == "DE");

    substr = ascii.substring(3, 3);

    REQUIRE(substr.size() == 3);
    REQUIRE(substr.toString() == "DEF");
}

TEST_CASE("UTF8String trim removes whitespace from both ends", "[UTF8String][trim]") {
    domain::UTF8String ascii("     ABC     ");

    domain::UTF8String trim = ascii.trim(domain::TrimDirection::Both);

    REQUIRE(trim.size() == 3);
    REQUIRE(trim.toString() == "ABC");
}

TEST_CASE("UTF8String trim removes whitespace from right only", "[UTF8String][trim]") {
    domain::UTF8String ascii("     ABC     ");

    domain::UTF8String trim = ascii.trim(domain::TrimDirection::Right);

    REQUIRE(trim.size() == 8);
    REQUIRE(trim.toString() == "     ABC");
}

TEST_CASE("UTF8String trim removes whitespace from left only", "[UTF8String][trim]") {
    domain::UTF8String ascii("     ABC     ");

    domain::UTF8String trim = ascii.trim(domain::TrimDirection::Left);

    REQUIRE(trim.size() == 8);
    REQUIRE(trim.toString() == "ABC     ");
}

TEST_CASE("UTF8String converts to std::string correctly", "[UTF8String][toString]") {
    domain::UTF8String test("THIS IS A TEST");

    REQUIRE(test.toString() == "THIS IS A TEST");
}

TEST_CASE("UTF8String concatenates with += operator", "[UTF8String][operator]") {
    domain::UTF8String test;
    test += domain::UTF8String("THIS ");
    test += domain::UTF8String("IS ");
    test += domain::UTF8String("A ");
    test += domain::UTF8String("TEST");

    REQUIRE(test.toString() == "THIS IS A TEST");
}

TEST_CASE("UTF8String compares equal with == operator", "[UTF8String][operator]") {
    domain::UTF8String test1("THIS IS A TEST");
    domain::UTF8String test2("THIS IS A TEST");

    REQUIRE(test1 == test2);
    REQUIRE(test1.toString() == test2.toString());
    REQUIRE(test1.size() == test2.size());

    for (int i = 0; i < test1.size(); i++) {
        REQUIRE(test1.at(i) == test2.at(i));
    }
}

TEST_CASE("UTF8String compares unequal strings correctly with != operator", "[UTF8String][operator]") {
    domain::UTF8String test1("THIS IS A TEST");
    domain::UTF8String test2("THIS IS DIFFERENT");
    REQUIRE(test1 != test2);
}
