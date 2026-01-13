#include "application/view/searchmenu.hpp"
#include "application/view/styling.hpp"
#include "application/view/widgets/input.hpp"
#include "application/view/widgets/scrolledmenuitems.hpp"

SearchMenuView::SearchMenuView() {

}

void SearchMenuView::init(domain::Renderer *renderer) {
    m_renderer = renderer;

    m_scrolled_menu_items = std::make_unique<ScrolledMenuItems>(MAX_VISIBLE_ITEMS);
    m_input = std::make_unique<Input>();
}

void SearchMenuView::render(const RenderContext &render_context, const domain::Menu &menu, int cursor_column) {
    domain::Font* font = render_context.ui_options.font_menu;

    m_input->setPrompt(menu.getInput().getPrompt());
    m_input->setContent(menu.getInput().getContent());

    m_x = render_context.screen_width - WIDTH;
    m_height = BORDER_THICKNESS + 2 * SPACING_MEDIUM + m_input->height(font);

    float items_size = menu.hasItems() ? std::min(MAX_VISIBLE_ITEMS, (int)menu.getItems().items.size()) : 0;
    if (items_size > 0)
    {
        m_height += BORDER_THICKNESS + font->getLineHeight() * items_size;
    }

    LayoutManager layout(m_x, 0, WIDTH, m_height);

    m_renderer->renderRoundedRectWithShadow(
        domain::RGBAColor{0.5, 0.5, 0.5, 1.0}, layout.current().x,
        layout.current().y, layout.current().width, layout.current().height, domain::CornerRadius(0, 0, CORNER_RADIUS, CORNER_RADIUS), SHADOW_LENGTH);

    layout.padLeft(BORDER_THICKNESS);
    layout.padRight(BORDER_THICKNESS);
    layout.padDown(BORDER_THICKNESS);

    if (items_size > 0) {
        m_renderer->renderRect(
            menu.getInputFace().getBg(render_context.default_face, render_context.ui_options.color_overrides), layout.current().x,
            layout.current().y, layout.current().width, SPACING_MEDIUM + m_input->height(font));
    } else {
        m_renderer->renderRoundedRect(
            menu.getInputFace().getBg(render_context.default_face, render_context.ui_options.color_overrides), layout.current().x,
            layout.current().y, layout.current().width, SPACING_MEDIUM + m_input->height(font) + SPACING_MEDIUM, domain::CornerRadius(0, 0, CORNER_RADIUS, CORNER_RADIUS));
    }

    layout.pad(SPACING_MEDIUM);

    m_input->render(m_renderer, render_context, menu.getInput(), menu.getInputFace(), cursor_column, layout);

    if (menu.hasItems())
    {
        m_renderer->renderRect(domain::RGBAColor{0.5, 0.5, 0.5, 1.0}, layout.current().x - SPACING_MEDIUM, layout.current().y, layout.current().width + SPACING_MEDIUM * 2, 1);

        layout.gapY(BORDER_THICKNESS);

        m_renderer->renderRoundedRect(
            menu.getItems().face.getBg(render_context.default_face, render_context.ui_options.color_overrides), layout.current().x - SPACING_MEDIUM,
            layout.current().y, layout.current().width + SPACING_MEDIUM*2, layout.current().height + SPACING_MEDIUM, domain::CornerRadius(0.0f, 0.0f, CORNER_RADIUS, CORNER_RADIUS));

        m_scrolled_menu_items->render(m_renderer, render_context, menu.getItems(), menu.getInputFace().getFg(render_context.default_face, render_context.ui_options.color_overrides), layout);
    }
}

float SearchMenuView::x() const {
    return m_x;
}

float SearchMenuView::y() const {
    return 0.0f;
}

float SearchMenuView::width() const {
    return WIDTH;
}

float SearchMenuView::height() const {
    return m_height;
}

float SearchMenuView::scrolledItemsX() const {
    return m_scrolled_menu_items->x();
}

float SearchMenuView::scrolledItemsY() const {
    return m_scrolled_menu_items->y();
}

float SearchMenuView::scrolledItemsWidth() const {
    return m_scrolled_menu_items->width();
}

float SearchMenuView::scrolledItemsHeight() const {
    return m_scrolled_menu_items->height();
}

domain::MouseMoveResult SearchMenuView::onMouseMove(float x, float y, const domain::Menu &menu) {
    if (menu.hasItems()) {
        float items_x = m_scrolled_menu_items->x();
        float items_y = m_scrolled_menu_items->y();
        float items_width = m_scrolled_menu_items->width();
        float items_height = m_scrolled_menu_items->height();

        if (x >= items_x && x < items_x + items_width && y >= items_y && y < items_y + items_height) {
            return domain::MouseMoveResult{domain::Cursor::POINTER};
        }
    }

    if (x >= this->x() && x < this->x() + width() && y >= this->y() && y < this->y() + height()) {
        return domain::MouseMoveResult{domain::Cursor::DEFAULT};
    }

    return domain::MouseMoveResult{std::nullopt};
}

std::optional<int> SearchMenuView::findItemAtPosition(float x, float y, const domain::Menu &menu) {
    if (!menu.hasItems()) return std::nullopt;
    return m_scrolled_menu_items->findItemAtPosition(x, y, menu.getItems());
}

void SearchMenuView::onMouseScroll(int scroll_amount, const domain::Menu &menu) {
    if (!menu.hasItems()) return;

    int total_items = menu.getItems().items.size();
    m_scrolled_menu_items->scroll(scroll_amount, total_items);
}

void SearchMenuView::ensureItemVisible(int index) {
    m_scrolled_menu_items->ensureItemVisible(index);
}
