#ifndef DOMAIN_FONTRESOLVER_HPP_INCLUDED
#define DOMAIN_FONTRESOLVER_HPP_INCLUDED

#include "domain/utf8string.hpp"
#include <optional>
#include <string>

namespace domain
{

enum class FontStyle
{
    Normal,
    Bold,
    Italic,
    BoldItalic
};

struct FontMatch
{
    std::string path;
    int size;
};

class FontResolver
{
  public:
    virtual ~FontResolver() = default;

    virtual FontMatch resolve(const std::string &pattern) = 0;
    virtual std::optional<FontMatch> resolveForCodepoint(Codepoint codepoint) = 0;
    virtual std::optional<FontMatch> resolveStyleVariant(const FontMatch &base_font, FontStyle style) = 0;
};

} // namespace domain

#endif
