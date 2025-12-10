#include "view/statusbar.hpp"
#include "core/alignment.hpp"

StatusBarView::StatusBarView() {

}

void StatusBarView::init(Renderer* renderer) {
    m_renderer = renderer;
}

void StatusBarView::render(Font* font, const kakoune::Line &mode, const kakoune::Face &default_face, float width, float height) {
    m_renderer->renderLine(font, mode, default_face, width, height, core::Alignment::bottomRight());
}

int StatusBarView::getCellWidth(Font* font) {
    return font->getGlyphMetrics('A').width();
}

int StatusBarView::getCellHeight(Font* font) {
    return font->getLineHeight();
}
