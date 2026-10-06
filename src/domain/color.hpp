#ifndef DOMAIN_COLOR_HPP_INCLUDED
#define DOMAIN_COLOR_HPP_INCLUDED

#include <unordered_map>
#include <variant>

namespace domain
{

struct DefaultColor
{
    bool operator==(const DefaultColor &) const;
};

enum class NamedColor
{
    Black,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White,
    BrightBlack,
    BrightRed,
    BrightGreen,
    BrightYellow,
    BrightBlue,
    BrightMagenta,
    BrightCyan,
    BrightWhite,
};

struct RGBAColor
{
    float r, g, b, a;

    bool operator==(const RGBAColor &other) const;
};

typedef std::variant<NamedColor, RGBAColor> ExplicitColor;

typedef std::variant<DefaultColor, NamedColor, RGBAColor> OptionalColor;

RGBAColor getDefaultBg();
RGBAColor getDefaultFg();
RGBAColor getRGBAColor(NamedColor color);
RGBAColor getRGBAColor(NamedColor color, const std::unordered_map<NamedColor, RGBAColor> &overrides);
RGBAColor getRGBAColor(OptionalColor color, RGBAColor fallback_color);

} // namespace domain

#endif
