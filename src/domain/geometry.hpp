#ifndef DOMAIN_GEOMETRY_HPP_INCLUDED
#define DOMAIN_GEOMETRY_HPP_INCLUDED

#include <optional>

namespace domain
{

struct IVec2
{
    int x;
    int y;
};

struct UIVec2
{
    unsigned int x;
    unsigned int y;
};

struct Vec2
{
    float x;
    float y;
};

class Rectangle
{
  public:
    Rectangle();
    Rectangle(int left, int top, int width, int height);

    std::optional<Rectangle> intersection(const Rectangle &rectangle) const;
    bool intersects(const Rectangle &rectangle) const;
    bool contains(int x, int y) const;

    float distance(int x, int y) const;
    float distanceSquared(int x, int y) const;

    void setX(int x);
    void setY(int y);
    void setWidth(int width);
    void setHeight(int height);

    int left() const;
    int right() const;
    int top() const;
    int bottom() const;
    int centerX() const;
    int centerY() const;

    int width() const;
    int height() const;

  private:
    int m_x, m_y, m_width, m_height;
};

} // namespace domain

#endif
