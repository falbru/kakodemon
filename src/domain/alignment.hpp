#ifndef DOMAIN_ALIGNMENT_HPP_INCLUDED
#define DOMAIN_ALIGNMENT_HPP_INCLUDED

namespace domain
{

struct Alignment
{
    enum class HorizontalAlignment
    {
        Left,
        Right
    } h = HorizontalAlignment::Left;
    enum class VerticalAlignment
    {
        Top,
        Bottom
    } v = VerticalAlignment::Top;

    Alignment() = default;
    Alignment(HorizontalAlignment horizontal, VerticalAlignment vertical);

    static Alignment topLeft();
    static Alignment topRight();
    static Alignment bottomLeft();
    static Alignment bottomRight();
};

} // namespace domain

#endif
