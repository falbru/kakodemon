#ifndef DOMAIN_EDITOR_HPP_INCLUDED
#define DOMAIN_EDITOR_HPP_INCLUDED

#include "domain/coord.hpp"
#include <variant>

namespace domain
{

struct BufferContentPosition
{
    domain::Coord coord;
};

struct StatusLinePosition
{
    int column;
};

using CursorPosition = std::variant<BufferContentPosition, StatusLinePosition>;

} // namespace domain

#endif // editor_hpp_INCLUDED
