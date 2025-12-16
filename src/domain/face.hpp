#ifndef DOMAIN_FACE_HPP_INCLUDED
#define DOMAIN_FACE_HPP_INCLUDED

#include "domain/color.hpp"

namespace domain
{

class Face
{
  public:
    Face(OptionalColor bg, OptionalColor fg);

    RGBAColor getFg(const Face &default_face) const;
    RGBAColor getBg(const Face &default_face) const;

    RGBAColor getFg() const;
    RGBAColor getBg() const;

  private:
    OptionalColor m_bg;
    OptionalColor m_fg;
};

} // namespace domain

#endif
