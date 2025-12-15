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

struct Rectangle
{
    float x;
    float y;
    float width;
    float height;

    bool intersects(const Rectangle &Rectangle) const;
};

} // namespace domain

#endif
