#include "domain/color.hpp"
#include <variant>

namespace domain
{

RGBAColor getDefaultFg()
{
    return RGBAColor{1.0f, 1.0f, 1.0f, 1.0f};
}

RGBAColor getDefaultBg()
{
    return RGBAColor{0.0f, 0.0f, 0.0f, 1.0f};
}

RGBAColor getRGBAColor(NamedColor color)
{
    switch (color)
    {
    case NamedColor::Black:
        return RGBAColor{0.0f, 0.0f, 0.0f, 1.0f};
        break;
    case NamedColor::Red:
        return RGBAColor{1.0f, 0.0f, 0.0f, 1.0f};
        break;
    case NamedColor::Green:
        return RGBAColor{0.0f, 0.502f, 0.0f, 1.0f};
        break;
    case NamedColor::Yellow:
        return RGBAColor{1.0f, 1.0f, 0.0f, 1.0f};
        break;
    case NamedColor::Blue:
        return RGBAColor{0.0f, 0.0f, 1.0f, 1.0f};
        break;
    case NamedColor::Magenta:
        return RGBAColor{1.0f, 0.0f, 1.0f, 1.0f};
        break;
    case NamedColor::Cyan:
        return RGBAColor{0.0f, 1.0f, 1.0f, 1.0f};
        break;
    case NamedColor::White:
        return RGBAColor{1.0f, 1.0f, 1.0f, 1.0f};
        break;
    case NamedColor::BrightBlack:
        return RGBAColor{0.502f, 0.502f, 0.502f, 1.0f};
        break;
    case NamedColor::BrightRed:
        return RGBAColor{1.0f, 0.502f, 0.502f, 1.0f};
        break;
    case NamedColor::BrightGreen:
        return RGBAColor{0.502f, 1.0f, 0.502f, 1.0f};
        break;
    case NamedColor::BrightYellow:
        return RGBAColor{1.0f, 1.0f, 0.502f, 1.0f};
        break;
    case NamedColor::BrightBlue:
        return RGBAColor{0.502f, 0.502f, 1.0f, 1.0f};
        break;
    case NamedColor::BrightMagenta:
        return RGBAColor{1.0f, 0.502f, 1.0f, 1.0f};
        break;
    case NamedColor::BrightCyan:
        return RGBAColor{0.502f, 1.0f, 1.0f, 1.0f};
        break;
    case NamedColor::BrightWhite:
        return RGBAColor{1.0f, 1.0f, 1.0f, 1.0f};
        break;
    }
}

RGBAColor getRGBAColor(NamedColor color, const std::unordered_map<NamedColor, RGBAColor> &overrides)
{
    auto it = overrides.find(color);
    if (it != overrides.end())
    {
        return it->second;
    }
    return getRGBAColor(color);
}

RGBAColor getRGBAColor(OptionalColor color, RGBAColor fallback_color)
{
    if (std::holds_alternative<RGBAColor>(color))
    {
        return std::get<RGBAColor>(color);
    }
    else if (std::holds_alternative<NamedColor>(color))
    {
        return getRGBAColor(std::get<NamedColor>(color));
    }
    else
    {
        return fallback_color;
    }
}

bool DefaultColor::operator==(const DefaultColor &) const
{
    return true;
}

bool RGBAColor::operator==(const RGBAColor &other) const
{
    return r == other.r && g == other.g && b == other.b && a == other.a;
}

} // namespace domain
