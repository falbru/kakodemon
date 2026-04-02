#ifndef DOMAIN_GEOMETRY_HPP_INCLUDED
#define DOMAIN_GEOMETRY_HPP_INCLUDED

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

struct Rectangle
{
    int x;
    int y;
    int width;
    int height;

    bool intersects(const Rectangle &Rectangle) const;
    bool contains(int x, int y) const;
};

} // namespace domain

#endif
