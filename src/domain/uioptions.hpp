#ifndef MODEL_UIOPTIONS_HPP_INCLUDED
#define MODEL_UIOPTIONS_HPP_INCLUDED

#include "domain/color.hpp"
#include "domain/ports/font.hpp"

namespace domain
{

struct UIOptions
{
    Font *font = nullptr;
    OptionalColor color_border;
};

} // namespace domain

#endif
