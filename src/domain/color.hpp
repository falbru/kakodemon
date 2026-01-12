#ifndef DOMAIN_COLOR_HPP_INCLUDED
#define DOMAIN_COLOR_HPP_INCLUDED

#include <variant>

namespace domain
{

struct DefaultColor
{
    bool operator==(const DefaultColor &) const;
};

enum class FixedColor
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

typedef std::variant<FixedColor, RGBAColor> ExplicitColor;

typedef std::variant<DefaultColor, FixedColor, RGBAColor> OptionalColor;

RGBAColor getDefaultBg();
RGBAColor getDefaultFg();
RGBAColor getRGBAColor(FixedColor color);
RGBAColor getRGBAColor(OptionalColor color, RGBAColor fallback_color);

} // namespace domain

#endif
