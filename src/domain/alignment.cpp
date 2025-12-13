#include "alignment.hpp"

namespace domain{

Alignment::Alignment(HorizontalAlignment horizontal, VerticalAlignment vertical)
    : h(horizontal), v(vertical) {}

Alignment Alignment::topLeft() { return {HorizontalAlignment::Left, VerticalAlignment::Top}; }
Alignment Alignment::topRight() { return {HorizontalAlignment::Right, VerticalAlignment::Top}; }
Alignment Alignment::bottomLeft() { return {HorizontalAlignment::Left, VerticalAlignment::Bottom}; }
Alignment Alignment::bottomRight() { return {HorizontalAlignment::Right, VerticalAlignment::Bottom}; }

}
