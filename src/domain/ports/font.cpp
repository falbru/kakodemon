#include "font.hpp"

namespace domain {

void loadBasicGlyphs(Font* font) {
    for (domain::Codepoint c = 0x20; c < 128; c++)
    {
        if (c == 0x7F) continue;

        font->loadGlyph(c);
    }
}

}
