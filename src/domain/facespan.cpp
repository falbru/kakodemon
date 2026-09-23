#include "facespan.hpp"

domain::FaceSpan::FaceSpan(domain::Face face, size_t start_index) : face(face), start_index(start_index)
{
}

bool domain::FaceSpan::operator<(const FaceSpan &other) const
{
    return start_index < other.start_index;
}
