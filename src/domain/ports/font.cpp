#include "domain/ports/font.hpp"

float domain::GlyphMetrics::width() const {
    return advance >> 6;
}
