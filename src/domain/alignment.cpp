#include "alignment.hpp"

namespace domain{

Alignment::Alignment(HorizontalAlignment horizontal, VerticalAlignment vertical)
    : h(horizontal), v(vertical) {}

domain::Alignment domain::Alignment::topLeft() { return {HorizontalAlignment::Left, VerticalAlignment::Top}; }
domain::Alignment domain::Alignment::topRight() { return {HorizontalAlignment::Right, VerticalAlignment::Top}; }
domain::Alignment domain::Alignment::bottomLeft() { return {HorizontalAlignment::Left, VerticalAlignment::Bottom}; }
domain::Alignment domain::Alignment::bottomRight() { return {HorizontalAlignment::Right, VerticalAlignment::Bottom}; }

}
