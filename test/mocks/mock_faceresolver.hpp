#ifndef MOCK_FACERESOLVER_HPP_INCLUDED
#define MOCK_FACERESOLVER_HPP_INCLUDED

#include "domain/face.hpp"
#include "domain/faceresolver.hpp"
#include <functional>

class FaceResolverMock : public domain::FaceResolver
{

  public:
    FaceResolverMock(domain::ResolvedFace resolved_face);
    FaceResolverMock(std::function<domain::ResolvedFace(const domain::Face &face)> resolver_function);

    domain::ResolvedFace resolve(const domain::Face &face) const override;

  private:
    std::function<domain::ResolvedFace(const domain::Face &face)> m_resolver_function;
};

#endif
