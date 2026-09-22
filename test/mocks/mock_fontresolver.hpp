#ifndef TEST_MOCK_FONTRESOLVER_HPP_INCLUDED
#define TEST_MOCK_FONTRESOLVER_HPP_INCLUDED

#include "domain/ports/fontresolver.hpp"
#include <optional>

class FontResolverMock : public domain::FontResolver
{
  public:
    domain::FontMatch resolve(const std::string &) override
    {
        return {"", 14, 0};
    }

    domain::FontMatch resolveDefault(int size) override
    {
        return {"", size, 0};
    }

    std::optional<domain::FontMatch> resolveForCodepoint(domain::Codepoint) override
    {
        return std::nullopt;
    }

    std::optional<domain::FontMatch> resolveStyleVariant(const domain::FontMatch &, domain::FontStyle) override
    {
        return std::nullopt;
    }
};

#endif
