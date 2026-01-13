#include "statusbar.hpp"
#include "application/view/layoutmanager.hpp"
#include "application/view/styling.hpp"
#include "domain/modeline.hpp"

StatusBarView::StatusBarView() {

}

void StatusBarView::init(domain::Renderer* renderer) {
    m_renderer = renderer;
}

void StatusBarView::render(const RenderContext &render_context, const domain::ModeLine& mode_line) {
    float bar_height = this->height(render_context.ui_options.font_statusbar);

    LayoutManager layout(0, render_context.screen_height - bar_height, render_context.screen_width, bar_height);

    m_renderer->renderRect(mode_line.getDefaultFace().getBg(render_context.ui_options.color_overrides), layout.current().x, layout.current().y, layout.current().width, layout.current().height);

    layout.pad(SPACING_SMALL);

    m_renderer->renderLine(render_context.textConfig(render_context.ui_options.font_statusbar), mode_line.getModeLine(), mode_line.getDefaultFace(), layout.current().x + layout.current().width, layout.current().y, domain::Alignment::topRight());
}

float StatusBarView::height(domain::Font* font) {
    return SPACING_SMALL * 2 + font->getLineHeight();
}
