#include <catch2/catch_test_macros.hpp>
#include "adapters/fontconfig/fontconfigresolver.hpp"
#include "domain/ports/fontresolver.hpp"

TEST_CASE("FontconfigResolver can be constructed", "[FontconfigResolver]")
{
    FontconfigResolver resolver;
    REQUIRE(true);
}

TEST_CASE("FontconfigResolver resolves basic monospace pattern", "[FontconfigResolver]")
{
    FontconfigResolver resolver;
    auto result = resolver.resolve("monospace");

    REQUIRE(!result.path.empty());
}

TEST_CASE("FontconfigResolver resolves pattern with size", "[FontconfigResolver]")
{
    FontconfigResolver resolver;
    auto result = resolver.resolve("monospace 12");

    REQUIRE(!result.path.empty());
    REQUIRE(result.size == 12);
}

TEST_CASE("FontconfigResolver resolves pattern with different sizes", "[FontconfigResolver]")
{
    FontconfigResolver resolver;

    auto small = resolver.resolve("sans 8");
    auto medium = resolver.resolve("sans 14");
    auto large = resolver.resolve("sans 24");

    REQUIRE(small.size == 8);
    REQUIRE(medium.size == 14);
    REQUIRE(large.size == 24);
}

TEST_CASE("FontconfigResolver resolves sans-serif pattern", "[FontconfigResolver]")
{
    FontconfigResolver resolver;
    auto result = resolver.resolve("sans-serif");

    REQUIRE(!result.path.empty());
}

TEST_CASE("FontconfigResolver resolves serif pattern", "[FontconfigResolver]")
{
    FontconfigResolver resolver;
    auto result = resolver.resolve("serif");

    REQUIRE(!result.path.empty());
}

TEST_CASE("FontconfigResolver handles pattern with multiple attributes", "[FontconfigResolver]")
{
    FontconfigResolver resolver;
    auto result = resolver.resolve("monospace 16");

    REQUIRE(!result.path.empty());
    REQUIRE(result.size == 16);
}

TEST_CASE("FontconfigResolver resolves font for ASCII codepoint", "[FontconfigResolver]")
{
    FontconfigResolver resolver;
    domain::Codepoint letter{0x0041}; // 'A'

    auto result = resolver.resolveForCodepoint(letter);

    REQUIRE(result.has_value());
    REQUIRE(!result->path.empty());
}

TEST_CASE("FontconfigResolver resolves font for emoji codepoint", "[FontconfigResolver]")
{
    FontconfigResolver resolver;
    domain::Codepoint emoji{0x1F606}; // 😆

    auto result = resolver.resolveForCodepoint(emoji);

    REQUIRE(result.has_value());
    REQUIRE(!result->path.empty());
}

TEST_CASE("FontconfigResolver resolves font for Latin Extended codepoint", "[FontconfigResolver]")
{
    FontconfigResolver resolver;
    domain::Codepoint letter{0x00E9}; // é

    auto result = resolver.resolveForCodepoint(letter);

    REQUIRE(result.has_value());
    REQUIRE(!result->path.empty());
}

TEST_CASE("FontconfigResolver resolves font for Cyrillic codepoint", "[FontconfigResolver]")
{
    FontconfigResolver resolver;
    domain::Codepoint cyrillic{0x0416}; // Ж

    auto result = resolver.resolveForCodepoint(cyrillic);

    REQUIRE(result.has_value());
    REQUIRE(!result->path.empty());
}

TEST_CASE("FontconfigResolver resolves font for CJK codepoint", "[FontconfigResolver]")
{
    FontconfigResolver resolver;
    domain::Codepoint cjk{0x4E2D}; // 中

    auto result = resolver.resolveForCodepoint(cjk);

    REQUIRE(result.has_value());
    REQUIRE(!result->path.empty());
}

TEST_CASE("FontconfigResolver resolves font for Greek codepoint", "[FontconfigResolver]")
{
    FontconfigResolver resolver;
    domain::Codepoint greek{0x03B1}; // α

    auto result = resolver.resolveForCodepoint(greek);

    REQUIRE(result.has_value());
    REQUIRE(!result->path.empty());
}

TEST_CASE("FontconfigResolver returns same result for identical patterns", "[FontconfigResolver]")
{
    FontconfigResolver resolver;

    auto result1 = resolver.resolve("monospace 12");
    auto result2 = resolver.resolve("monospace 12");

    REQUIRE(result1.path == result2.path);
    REQUIRE(result1.size == result2.size);
}

TEST_CASE("FontconfigResolver handles zero size", "[FontconfigResolver]")
{
    FontconfigResolver resolver;
    auto result = resolver.resolve("monospace 0");

    REQUIRE(!result.path.empty());
}

TEST_CASE("FontconfigResolver handles nonexistent font name", "[FontconfigResolver]")
{
    FontconfigResolver resolver;
    auto result = resolver.resolve("ThisFontDoesNotExist123456789");

    // Fontconfig should provide a fallback
    REQUIRE(!result.path.empty());
}

TEST_CASE("FontconfigResolver multiple instances work independently", "[FontconfigResolver]")
{
    FontconfigResolver resolver1;
    FontconfigResolver resolver2;

    auto result1 = resolver1.resolve("monospace");
    auto result2 = resolver2.resolve("sans-serif");

    REQUIRE(!result1.path.empty());
    REQUIRE(!result2.path.empty());
}

TEST_CASE("FontconfigResolver handles smiley emoji", "[FontconfigResolver]")
{
    FontconfigResolver resolver;
    domain::Codepoint smiley{0x263A}; // ☺

    auto result = resolver.resolveForCodepoint(smiley);

    REQUIRE(result.has_value());
    REQUIRE(!result->path.empty());
}

TEST_CASE("FontconfigResolver handles heart emoji", "[FontconfigResolver]")
{
    FontconfigResolver resolver;
    domain::Codepoint heart{0x2764}; // ❤

    auto result = resolver.resolveForCodepoint(heart);

    REQUIRE(result.has_value());
    REQUIRE(!result->path.empty());
}


TEST_CASE("FontconfigResolver handles rapid successive calls", "[FontconfigResolver]")
{
    FontconfigResolver resolver;

    for (int i = 0; i < 50; ++i)
    {
        auto result = resolver.resolve("monospace");
        REQUIRE(!result.path.empty());
    }
}

TEST_CASE("FontconfigResolver resolves different codepoints consecutively", "[FontconfigResolver]")
{
    FontconfigResolver resolver;

    domain::Codepoint latin{0x0041};    // A
    domain::Codepoint cyrillic{0x0410}; // А
    domain::Codepoint greek{0x0391};    // Α

    auto result1 = resolver.resolveForCodepoint(latin);
    auto result2 = resolver.resolveForCodepoint(cyrillic);
    auto result3 = resolver.resolveForCodepoint(greek);

    REQUIRE(result1.has_value());
    REQUIRE(result2.has_value());
    REQUIRE(result3.has_value());
}

TEST_CASE("FontconfigResolver handles space codepoint", "[FontconfigResolver]")
{
    FontconfigResolver resolver;
    domain::Codepoint space{0x0020}; // ' '

    auto result = resolver.resolveForCodepoint(space);

    REQUIRE(result.has_value());
    REQUIRE(!result->path.empty());
}

TEST_CASE("FontconfigResolver handles digit codepoint", "[FontconfigResolver]")
{
    FontconfigResolver resolver;
    domain::Codepoint digit{0x0039}; // '9'

    auto result = resolver.resolveForCodepoint(digit);

    REQUIRE(result.has_value());
    REQUIRE(!result->path.empty());
}
