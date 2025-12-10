#include "font.hpp"

float GlyphMetrics::width() const {
    return advance >> 6;
}
