#ifndef DOMAIN_KEY_HPP_INCLUDED
#define DOMAIN_KEY_HPP_INCLUDED

#include "utf8string.hpp"
#include <variant>

namespace domain
{

enum class SpecialKey
{
    ESCAPE,
    TAB,
    RETURN,
    BACKSPACE,
    DELETE,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    HOME,
    END,
    PAGE_UP,
    PAGE_DOWN,
    INSERT,
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12
};

enum KeyModifier
{
    NONE = 0,
    SHIFT = 1 << 0,
    CONTROL = 1 << 1,
    ALT = 1 << 2,
};

using Key = std::variant<Codepoint, SpecialKey>;

struct KeyEvent
{
    Key key;
    int modifiers;
};

} // namespace domain

#endif
