#ifndef KEY_HPP_INCLUDED
#define KEY_HPP_INCLUDED

#include "core/utf8string.hpp"
#include <variant>

namespace core
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
    INSERT
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

} // namespace core

#endif
