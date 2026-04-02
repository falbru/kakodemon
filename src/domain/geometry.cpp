#include "domain/geometry.hpp"

namespace domain {

bool Rectangle::intersects(const Rectangle& other) const {
    return this->x < other.x + other.width &&
           this->x + this->width > other.x &&
           this->y < other.y + other.height &&
           this->y + this->height > other.y;
}

bool Rectangle::contains(int x, int y) const {
    if (this->width == 0 || this->height == 0) return false;

    return this->x <= x && x <= this->x + this->width && this->y <= y && y <= this->y + this->height;
}

}
