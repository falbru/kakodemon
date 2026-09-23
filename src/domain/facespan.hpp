#ifndef FACESPAN_HPP_INCLUDED
#define FACESPAN_HPP_INCLUDED

#include "domain/face.hpp"
#include <cstddef>

namespace domain
{

struct FaceSpan
{
    FaceSpan(Face face, size_t start_index);

    Face face;
    size_t start_index;

    bool operator<(const FaceSpan &other) const;
};

} // namespace domain

#endif
