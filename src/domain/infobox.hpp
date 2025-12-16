#ifndef DOMAIN_INFOBOX_HPP_INCLUDED
#define DOMAIN_INFOBOX_HPP_INCLUDED

#include "domain/coord.hpp"
#include "domain/face.hpp"
#include "domain/line.hpp"
#include "domain/lines.hpp"

namespace domain
{

enum class InfoStyle
{
    PROMPT,
    INLINE,
    INLINE_ABOVE,
    INLINE_BELOW,
    MENUDOC,
    MODAL,
};

struct InfoBox
{
    Line title;
    Lines content;
    Coord anchor;
    Face default_face;
    InfoStyle style;
};

} // namespace domain

#endif
