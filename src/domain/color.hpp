#ifndef DOMAIN_COLOR_HPP_INCLUDED
#define DOMAIN_COLOR_HPP_INCLUDED

#include <variant>

namespace domain
{

struct DefaultColor
{
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
};

typedef std::variant<FixedColor, RGBAColor> ExplicitColor;

typedef std::variant<DefaultColor, FixedColor, RGBAColor> OptionalColor;

RGBAColor getDefaultBg();
RGBAColor getDefaultFg();
RGBAColor getRGBAColor(FixedColor color);

} // namespace domain

#endif
