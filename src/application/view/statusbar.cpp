#include "statusbar.hpp"
#include "application/view/layoutmanager.hpp"
#include "application/view/styling.hpp"
#include "domain/alignment.hpp"
#include "domain/modeline.hpp"

StatusBarView::StatusBarView() {

}

void StatusBarView::init(domain::Renderer* renderer) {
    m_renderer = renderer;
}

void StatusBarView::render(domain::Font* font, domain::FontManager* font_manager, const domain::ModeLine& mode_line, float width, float height) {
    float bar_height = this->height(font);

    LayoutManager layout(0, height - bar_height, width, bar_height);

    m_renderer->renderRect(mode_line.getDefaultFace().getBg(), layout.current().x, layout.current().y, layout.current().width, layout.current().height); // TODO use default_face as fallback

    layout.pad(SPACING_SMALL);

    m_renderer->renderLine(font, font_manager, mode_line.getModeLine(), mode_line.getDefaultFace(), layout.current().x + layout.current().width, layout.current().y, domain::Alignment::topRight());
}

float StatusBarView::height(domain::Font* font) {
    return SPACING_SMALL * 2 + font->getLineHeight();
}
