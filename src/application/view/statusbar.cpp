#include "statusbar.hpp"
#include "domain/alignment.hpp"

StatusBarView::StatusBarView() {

}

void StatusBarView::init(domain::Renderer* renderer) {
    m_renderer = renderer;
}

void StatusBarView::render(domain::Font* font, const kakoune::Line &mode, const kakoune::Face &default_face, float width, float height) {
    m_renderer->renderLine(font, mode, default_face, width, height, domain::Alignment::bottomRight());
}

int StatusBarView::getCellWidth(domain::Font* font) {
    return font->getGlyphMetrics('A').width();
}

int StatusBarView::getCellHeight(domain::Font* font) {
    return font->getLineHeight();
}
