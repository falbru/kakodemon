#ifndef EDITOR_HPP_INCLUDED
#define EDITOR_HPP_INCLUDED

#include "kakoune/coord.hpp"
#include "kakoune/line.hpp"
#include <variant>

namespace domain
{

struct BufferContentPosition
{
    kakoune::Coord coord;
};

struct StatusLinePosition
{
    int column;
};

using CursorPosition = std::variant<BufferContentPosition, StatusLinePosition>;

struct StatusLine
{
    kakoune::Line prompt;
    kakoune::Line content;
};

} // namespace domain

#endif // editor_hpp_INCLUDED
