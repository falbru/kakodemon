#include <catch2/catch_test_macros.hpp>

#include "domain/line.hpp"
#include "domain/atom.hpp"
#include "domain/utf8string.hpp"

TEST_CASE("Line slice with single atom returns correct substring", "[Line][slice]") {
    domain::Line line({
        domain::Atom(domain::UTF8String("ABCDEFGHIJ"), domain::Face(domain::DefaultColor(), domain::DefaultColor()))
    });

    domain::Line sliced = line.slice(0, 5);

    REQUIRE(sliced.length() == 5);
    REQUIRE(sliced.toUTF8String().toString() == "ABCDE");
}

TEST_CASE("Line slice with single atom from middle", "[Line][slice]") {
    domain::Line line({
        domain::Atom(domain::UTF8String("ABCDEFGHIJ"), domain::Face(domain::DefaultColor(), domain::DefaultColor()))
    });

    domain::Line sliced = line.slice(3, 4);

    REQUIRE(sliced.length() == 4);
    REQUIRE(sliced.toUTF8String().toString() == "DEFG");
}

TEST_CASE("Line slice with multiple atoms returns correct substring", "[Line][slice]") {
    domain::Line line({
        domain::Atom(domain::UTF8String("ABCDE"), domain::Face(domain::DefaultColor(), domain::DefaultColor())),
        domain::Atom(domain::UTF8String("FGHIJ"), domain::Face(domain::DefaultColor(), domain::DefaultColor())),
        domain::Atom(domain::UTF8String("KLMNO"), domain::Face(domain::DefaultColor(), domain::DefaultColor()))
    });

    domain::Line sliced = line.slice(0, 10);

    REQUIRE(sliced.length() == 10);
    REQUIRE(sliced.toUTF8String().toString() == "ABCDEFGHIJ");
}

TEST_CASE("Line slice spanning multiple atoms", "[Line][slice]") {
    domain::Line line({
        domain::Atom(domain::UTF8String("ABCDE"), domain::Face(domain::DefaultColor(), domain::DefaultColor())),
        domain::Atom(domain::UTF8String("FGHIJ"), domain::Face(domain::DefaultColor(), domain::DefaultColor())),
        domain::Atom(domain::UTF8String("KLMNO"), domain::Face(domain::DefaultColor(), domain::DefaultColor()))
    });

    domain::Line sliced = line.slice(3, 7);

    REQUIRE(sliced.length() == 7);
    REQUIRE(sliced.toUTF8String().toString() == "DEFGHIJ");
}

TEST_CASE("Line slice starting in middle of second atom", "[Line][slice]") {
    domain::Line line({
        domain::Atom(domain::UTF8String("ABCDE"), domain::Face(domain::DefaultColor(), domain::DefaultColor())),
        domain::Atom(domain::UTF8String("FGHIJ"), domain::Face(domain::DefaultColor(), domain::DefaultColor())),
        domain::Atom(domain::UTF8String("KLMNO"), domain::Face(domain::DefaultColor(), domain::DefaultColor()))
    });

    domain::Line sliced = line.slice(7, 5);

    REQUIRE(sliced.length() == 5);
    REQUIRE(sliced.toUTF8String().toString() == "HIJKL");
}

TEST_CASE("Line slice with zero length returns empty line", "[Line][slice]") {
    domain::Line line({
        domain::Atom(domain::UTF8String("ABCDE"), domain::Face(domain::DefaultColor(), domain::DefaultColor()))
    });

    domain::Line sliced = line.slice(2, 0);

    REQUIRE(sliced.length() == 0);
    REQUIRE(sliced.size() == 0);
}

TEST_CASE("Line slice with negative length returns empty line", "[Line][slice]") {
    domain::Line line({
        domain::Atom(domain::UTF8String("ABCDE"), domain::Face(domain::DefaultColor(), domain::DefaultColor()))
    });

    domain::Line sliced = line.slice(2, -5);

    REQUIRE(sliced.length() == 0);
    REQUIRE(sliced.size() == 0);
}

TEST_CASE("Line slice starting beyond end returns empty line", "[Line][slice]") {
    domain::Line line({
        domain::Atom(domain::UTF8String("ABCDE"), domain::Face(domain::DefaultColor(), domain::DefaultColor()))
    });

    domain::Line sliced = line.slice(10, 5);

    REQUIRE(sliced.length() == 0);
    REQUIRE(sliced.size() == 0);
}

TEST_CASE("Line slice with length extending beyond content stops at end", "[Line][slice]") {
    domain::Line line({
        domain::Atom(domain::UTF8String("ABCDE"), domain::Face(domain::DefaultColor(), domain::DefaultColor())),
        domain::Atom(domain::UTF8String("FGHIJ"), domain::Face(domain::DefaultColor(), domain::DefaultColor()))
    });

    domain::Line sliced = line.slice(7, 100);

    REQUIRE(sliced.length() == 3);
    REQUIRE(sliced.toUTF8String().toString() == "HIJ");
}

TEST_CASE("Line slice entire line returns complete copy", "[Line][slice]") {
    domain::Line line({
        domain::Atom(domain::UTF8String("ABCDE"), domain::Face(domain::DefaultColor(), domain::DefaultColor())),
        domain::Atom(domain::UTF8String("FGHIJ"), domain::Face(domain::DefaultColor(), domain::DefaultColor()))
    });

    domain::Line sliced = line.slice(0, 10);

    REQUIRE(sliced.length() == 10);
    REQUIRE(sliced.toUTF8String().toString() == "ABCDEFGHIJ");
}

TEST_CASE("Line slice with single character", "[Line][slice]") {
    domain::Line line({
        domain::Atom(domain::UTF8String("ABCDEFGHIJ"), domain::Face(domain::DefaultColor(), domain::DefaultColor()))
    });

    domain::Line sliced = line.slice(5, 1);

    REQUIRE(sliced.length() == 1);
    REQUIRE(sliced.toUTF8String().toString() == "F");
}

TEST_CASE("Line slice at exact atom boundary", "[Line][slice]") {
    domain::Line line({
        domain::Atom(domain::UTF8String("ABCDE"), domain::Face(domain::DefaultColor(), domain::DefaultColor())),
        domain::Atom(domain::UTF8String("FGHIJ"), domain::Face(domain::DefaultColor(), domain::DefaultColor()))
    });

    domain::Line sliced = line.slice(5, 3);

    REQUIRE(sliced.length() == 3);
    REQUIRE(sliced.toUTF8String().toString() == "FGH");
}

TEST_CASE("Line slice from empty line returns empty", "[Line][slice]") {
    domain::Line line;

    domain::Line sliced = line.slice(0, 5);

    REQUIRE(sliced.length() == 0);
    REQUIRE(sliced.size() == 0);
}

TEST_CASE("Line slice does not include characters beyond requested length", "[Line][slice]") {
    domain::Line line({
        domain::Atom(domain::UTF8String("0123456789"), domain::Face(domain::DefaultColor(), domain::DefaultColor())),
        domain::Atom(domain::UTF8String("ABCDEFGHIJ"), domain::Face(domain::DefaultColor(), domain::DefaultColor())),
        domain::Atom(domain::UTF8String("KLMNOPQRST"), domain::Face(domain::DefaultColor(), domain::DefaultColor())),
        domain::Atom(domain::UTF8String("UVWXYZ"), domain::Face(domain::DefaultColor(), domain::DefaultColor()))
    });

    REQUIRE(line.length() == 36);

    domain::Line sliced = line.slice(0, 10);

    REQUIRE(sliced.length() == 10);
    REQUIRE(sliced.toUTF8String().toString() == "0123456789");
}

TEST_CASE("Line slice with cursor position scenario", "[Line][slice]") {
    domain::Line content_line({
        domain::Atom(domain::UTF8String("This is a test input with many characters to test scrolling in the input widget component"), domain::Face(domain::DefaultColor(), domain::DefaultColor()))
    });

    REQUIRE(content_line.length() == 89);

    int cursor_pos_column = 40;
    domain::Line up_to_cursor = content_line.slice(0, cursor_pos_column);
    domain::Line up_to_cursor_plus_one = content_line.slice(0, cursor_pos_column + 1);

    REQUIRE(up_to_cursor.length() == 40);
    REQUIRE(up_to_cursor_plus_one.length() == 41);
    REQUIRE(up_to_cursor_plus_one.toUTF8String().toString() == "This is a test input with many characters");
}
