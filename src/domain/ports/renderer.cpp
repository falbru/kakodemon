#include "domain/ports/renderer.hpp"

namespace domain {
    CornerRadius::CornerRadius(float radius)
        : top_left(radius)
        , top_right(radius)
        , bottom_right(radius)
        , bottom_left(radius)
    {}

    CornerRadius::CornerRadius(float top_left, float top_right, float bottom_right, float bottom_left)
        : top_left(top_left)
        , top_right(top_right)
        , bottom_right(bottom_right)
        , bottom_left(bottom_left)
    {}

}
