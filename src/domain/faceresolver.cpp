#include "domain/faceresolver.hpp"
#include "domain/face.hpp"
#include <optional>

namespace domain
{

FaceResolver::FaceResolver(std::optional<Face> fallback_face, DefaultFace default_face, ColorOverrides color_overrides)
    : m_fallback_face(fallback_face), m_default_face(default_face), m_color_overrides(color_overrides)
{
}

ResolvedFace FaceResolver::resolve(const Face &face) const
{
    if (m_fallback_face.has_value())
    {
        return face.resolve(m_fallback_face.value(), m_default_face, m_color_overrides);
    }

    return face.resolve(m_default_face, m_color_overrides);
}

} // namespace domain
