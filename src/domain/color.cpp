#include "domain/color.hpp"

namespace domain {

RGBAColor getDefaultFg() {
    return RGBAColor{1.0f, 1.0f, 1.0f, 1.0f};
}

RGBAColor getDefaultBg() {
    return RGBAColor{0.0f, 0.0f, 0.0f, 1.0f};
}

RGBAColor getRGBAColor(FixedColor color) {
    switch(color) {
        case FixedColor::Black:
            return RGBAColor{0.0f, 0.0f, 0.0f, 1.0f};
            break;
        case FixedColor::Red:
            return RGBAColor{1.0f, 0.0f, 0.0f, 1.0f};
            break;
        case FixedColor::Green:
            return RGBAColor{0.0f, 0.502f, 0.0f, 1.0f};
            break;
        case FixedColor::Yellow:
            return RGBAColor{1.0f, 1.0f, 0.0f, 1.0f};
            break;
        case FixedColor::Blue:
            return RGBAColor{0.0f, 0.0f, 1.0f, 1.0f};
            break;
        case FixedColor::Magenta:
            return RGBAColor{1.0f, 0.0f, 1.0f, 1.0f};
            break;
        case FixedColor::Cyan:
            return RGBAColor{0.0f, 1.0f, 1.0f, 1.0f};
            break;
        case FixedColor::White:
            return RGBAColor{1.0f, 1.0f, 1.0f, 1.0f};
            break;
        case FixedColor::BrightBlack:
            return RGBAColor{0.502f, 0.502f, 0.502f, 1.0f};
            break;
        case FixedColor::BrightRed:
            return RGBAColor{1.0f, 0.502f, 0.502f, 1.0f};
            break;
        case FixedColor::BrightGreen:
            return RGBAColor{0.502f, 1.0f, 0.502f, 1.0f};
            break;
        case FixedColor::BrightYellow:
            return RGBAColor{1.0f, 1.0f, 0.502f, 1.0f};
            break;
        case FixedColor::BrightBlue:
            return RGBAColor{0.502f, 0.502f, 1.0f, 1.0f};
            break;
        case FixedColor::BrightMagenta:
            return RGBAColor{1.0f, 0.502f, 1.0f, 1.0f};
            break;
        case FixedColor::BrightCyan:
            return RGBAColor{0.502f, 1.0f, 1.0f, 1.0f};
            break;
        case FixedColor::BrightWhite:
            return RGBAColor{1.0f, 1.0f, 1.0f, 1.0f};
            break;
    }
}

bool DefaultColor::operator==(const DefaultColor&) const {
    return true;
}

bool RGBAColor::operator==(const RGBAColor& other) const {
    return r == other.r && g == other.g && b == other.b && a == other.a;
}

}
