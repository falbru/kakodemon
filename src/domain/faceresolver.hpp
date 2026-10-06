#ifndef RGBAFACERESOLVER_HPP_INCLUDED
#define RGBAFACERESOLVER_HPP_INCLUDED

#include "domain/face.hpp"
#include "domain/uioptions.hpp"

namespace domain
{

class FaceResolver
{
  public:
    FaceResolver(std::optional<Face> fallback_face, DefaultFace default_face, ColorOverrides color_overrides);

    virtual ResolvedFace resolve(const Face &face) const;

  private:
    std::optional<Face> m_fallback_face;
    DefaultFace m_default_face;
    ColorOverrides m_color_overrides;
};

} // namespace domain

#endif
