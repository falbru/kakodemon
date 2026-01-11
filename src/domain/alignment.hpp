#ifndef DOMAIN_ALIGNMENT_HPP_INCLUDED
#define DOMAIN_ALIGNMENT_HPP_INCLUDED

namespace domain
{

struct Alignment
{
    enum class HorizontalAlignment
    {
        Left,
        Center,
        Right
    } h = HorizontalAlignment::Left;
    enum class VerticalAlignment
    {
        Top,
        Center,
        Bottom
    } v = VerticalAlignment::Top;

    Alignment() = default;
    Alignment(HorizontalAlignment horizontal, VerticalAlignment vertical);

    static Alignment topLeft();
    static Alignment topCenter();
    static Alignment topRight();
    static Alignment centerLeft();
    static Alignment centerCenter();
    static Alignment centerRight();
    static Alignment bottomLeft();
    static Alignment bottomCenter();
    static Alignment bottomRight();
};

} // namespace domain

#endif
