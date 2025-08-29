#include "alignment.hpp"

namespace core{

Alignment::Alignment(HorizontalAlignment horizontal, VerticalAlignment vertical)
    : h(horizontal), v(vertical) {}

core::Alignment core::Alignment::topLeft() { return {HorizontalAlignment::Left, VerticalAlignment::Top}; }
core::Alignment core::Alignment::topRight() { return {HorizontalAlignment::Right, VerticalAlignment::Top}; }
core::Alignment core::Alignment::bottomLeft() { return {HorizontalAlignment::Left, VerticalAlignment::Bottom}; }
core::Alignment core::Alignment::bottomRight() { return {HorizontalAlignment::Right, VerticalAlignment::Bottom}; }

}
