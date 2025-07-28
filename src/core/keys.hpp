#ifndef KEY_HPP_INCLUDED
#define KEY_HPP_INCLUDED

#include <string>
#include <variant>

enum SpecialKey
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
    INSERT
};

enum KeyModifier
{
    NONE = 0,
    SHIFT = 1 << 0,
    CONTROL = 1 << 1,
    ALT = 1 << 2,
};

using Key = std::variant<std::string, SpecialKey>;

struct KeyEvent
{
    Key key;
    int modifiers;
};

#endif
