#include "domain/ports/font.hpp"

float GlyphMetrics::width() const {
    return advance >> 6;
}
