#include "domain/geometry.hpp"

namespace domain {

bool Rectangle::intersects(const Rectangle& other) const {
    return this->x < other.x + other.width &&
           this->x + this->width > other.x &&
           this->y < other.y + other.height &&
           this->y + this->height > other.y;
}

}
