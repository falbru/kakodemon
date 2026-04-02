#ifndef DOMAIN_MOUSEMOVERESULT_HPP_INCLUDED
#define DOMAIN_MOUSEMOVERESULT_HPP_INCLUDED

#include <optional>

namespace domain
{

enum class Cursor
{
    DEFAULT,
    IBEAM,
    POINTER,
    CROSSHAIR,
};

enum class MouseButton
{
    LEFT,
    MIDDLE,
    RIGHT
};

enum class MouseButtonAction
{
    PRESS,
    RELEASE
};

struct MouseButtonEvent
{
    MouseButton button;
    MouseButtonAction action;
    int modifiers;
    float x;
    float y;
};

struct MouseMoveEvent
{
    float x;
    float y;
};

struct MouseScrollEvent
{
    double scroll_amount;
};

struct MouseMoveResult
{
    std::optional<Cursor> cursor;
};
} // namespace domain

#endif // DOMAIN_MOUSEMOVERESULT_HPP_INCLUDED
