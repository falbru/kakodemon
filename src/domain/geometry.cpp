#include "domain/geometry.hpp"
#include <algorithm>
#include <cmath>

namespace domain {

Rectangle::Rectangle() : m_x(0), m_y(0), m_width(0), m_height(0) {

}

Rectangle::Rectangle(int left, int top, int width, int height) : m_x(left), m_y(top), m_width(width), m_height(height) {
}

std::optional<Rectangle> Rectangle::intersection(const Rectangle& other) const {
    if (!intersects(other)) {
        return std::nullopt;
    }

    int left = std::max(this->left(), other.left());
    int right = std::min(this->right(), other.right());
    int top = std::max(this->top(), other.top());
    int bottom = std::min(this->bottom(), other.bottom());

    return Rectangle(left, top, right - left, bottom - top);
}

bool Rectangle::intersects(const Rectangle& other) const {
    if (m_width == 0 || m_height == 0 || other.width() == 0 || other.height() == 0) return false;

    return left() < other.right() &&
           right() > other.left() &&
           top() < other.bottom() &&
           bottom() > other.top();
}

bool Rectangle::contains(int x, int y) const {
    if (m_width == 0 || m_height == 0) return false;

    return left() <= x && x <= right() && top() <= y && y <= bottom();
}

int Rectangle::left() const {
    return m_x;
}

int Rectangle::right() const {
    return m_x + m_width;
}

int Rectangle::top() const {
    return m_y;
}

int Rectangle::bottom() const {
    return m_y + m_height;
}

int Rectangle::width() const {
    return m_width;
}

int Rectangle::height() const {
    return m_height;
}

int Rectangle::centerX() const {
    return m_x + m_width / 2;
}

int Rectangle::centerY() const {
    return m_y + m_height / 2;
}

float Rectangle::distance(int x, int y) const {
    return std::sqrt(distanceSquared(x, y));
}

float Rectangle::distanceSquared(int x, int y) const {
    if (contains(x, y)) {
        return 0.0f;
    }

    float dx = std::max(left() - x, 0);
    dx = std::max(dx, (float)(x - right()));

    float dy = std::max(top() - y, 0);
    dy = std::max(dy, (float)(y - bottom()));

    return dx * dx + dy * dy;
}

void Rectangle::setX(int x) {
    m_x = x;
}

void Rectangle::setY(int y) {
    m_y = y;
}

void Rectangle::setWidth(int width) {
    m_width = width;
}

void Rectangle::setHeight(int height) {
    m_height = height;
}

}
