#include "mock_faceresolver.hpp"
#include "domain/color.hpp"
#include "domain/face.hpp"
#include "domain/faceresolver.hpp"

FaceResolverMock::FaceResolverMock(domain::ResolvedFace resolved_face)
    : FaceResolverMock([resolved_face](const domain::Face &face) { return resolved_face; })
{
}

FaceResolverMock::FaceResolverMock(std::function<domain::ResolvedFace(const domain::Face &face)> resolver_function)
    : domain::FaceResolver(std::nullopt, domain::DefaultFace(domain::DefaultColor{}, domain::DefaultColor{}), {}),
      m_resolver_function(resolver_function)
{
}

domain::ResolvedFace FaceResolverMock::resolve(const domain::Face &face) const
{
    return m_resolver_function(face);
}
