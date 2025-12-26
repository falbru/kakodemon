#include "statusbar.hpp"
#include "domain/alignment.hpp"
#include "domain/modeline.hpp"

StatusBarView::StatusBarView() {

}

void StatusBarView::init(domain::Renderer* renderer) {
    m_renderer = renderer;
}

void StatusBarView::render(domain::Font* font, domain::FontManager* font_manager, const domain::ModeLine& mode_line, float width, float height) {
    m_renderer->renderLine(font, font_manager, mode_line.getModeLine(), mode_line.getDefaultFace(), width, height, domain::Alignment::bottomRight());
}

int StatusBarView::getCellWidth(domain::Font* font) {
    return font->getGlyphMetrics('A').advance;
}

int StatusBarView::getCellHeight(domain::Font* font) {
    return font->getLineHeight();
}
