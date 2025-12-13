#ifndef DOMAIN_MOUSEMOVERESULT_HPP_INCLUDED
#define DOMAIN_MOUSEMOVERESULT_HPP_INCLUDED

#include <optional>

namespace domain
{
enum class Cursor
{
    DEFAULT,
    IBEAM,
    POINTER
};

struct MouseMoveResult
{
    std::optional<Cursor> cursor;
};
} // namespace domain

#endif // DOMAIN_MOUSEMOVERESULT_HPP_INCLUDED
