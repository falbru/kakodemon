#include "alignment.hpp"

namespace domain{

Alignment::Alignment(HorizontalAlignment horizontal, VerticalAlignment vertical)
    : h(horizontal), v(vertical) {}

Alignment Alignment::topLeft() { return {HorizontalAlignment::Left, VerticalAlignment::Top}; }
Alignment Alignment::topCenter() { return {HorizontalAlignment::Center, VerticalAlignment::Top}; }
Alignment Alignment::topRight() { return {HorizontalAlignment::Right, VerticalAlignment::Top}; }
Alignment Alignment::centerLeft() { return {HorizontalAlignment::Left, VerticalAlignment::Center}; }
Alignment Alignment::centerCenter() { return {HorizontalAlignment::Center, VerticalAlignment::Center}; }
Alignment Alignment::centerRight() { return {HorizontalAlignment::Right, VerticalAlignment::Center}; }
Alignment Alignment::bottomLeft() { return {HorizontalAlignment::Left, VerticalAlignment::Bottom}; }
Alignment Alignment::bottomCenter() { return {HorizontalAlignment::Center, VerticalAlignment::Bottom}; }
Alignment Alignment::bottomRight() { return {HorizontalAlignment::Right, VerticalAlignment::Bottom}; }

}
