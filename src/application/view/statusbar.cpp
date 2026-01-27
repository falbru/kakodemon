#include "statusbar.hpp"
#include "application/view/layoutmanager.hpp"
#include "application/view/styling.hpp"
#include "domain/editor.hpp"
#include "domain/modeline.hpp"
#include <variant>

StatusBarView::StatusBarView() {

}

void StatusBarView::init(domain::Renderer* renderer) {
    m_renderer = renderer;
}

void StatusBarView::render(const RenderContext &render_context, InputViewState &state, const domain::ModeLine& mode_line, domain::CursorPosition cursor_position) {
    float bar_height = this->height(render_context.ui_options.font_statusbar);

    LayoutManager layout(0, render_context.screen_height - bar_height, render_context.screen_width, bar_height);

    m_renderer->renderRect(mode_line.getDefaultFace().getBg(render_context.ui_options.color_overrides), layout.current().x, layout.current().y, layout.current().width, layout.current().height);

    layout.pad(SPACING_SMALL);

    m_renderer->renderLine(render_context.textConfig(render_context.ui_options.font_statusbar), mode_line.getModeLine(), mode_line.getDefaultFace(), layout.current().x + layout.current().width, layout.current().y, domain::Alignment::topRight());

    if (mode_line.getStatusLine().has_value()) {
        int cursor_column = 0;

        if (std::holds_alternative<domain::StatusLinePosition>(cursor_position))
            cursor_column = std::get<domain::StatusLinePosition>(cursor_position).column;

        m_input_widget.render(m_renderer, render_context, render_context.ui_options.font_statusbar, mode_line.getStatusLine().value(), mode_line.getDefaultFace(), cursor_column, state, layout);
    }
}

float StatusBarView::height(domain::Font* font) {
    return SPACING_SMALL * 2 + font->getLineHeight();
}
