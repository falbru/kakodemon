#include "promptmenu.hpp"
#include "application/view/rendercontext.hpp"
#include "domain/color.hpp"
#include "domain/ports/renderer.hpp"
#include "domain/uioptions.hpp"
#include "styling.hpp"
#include "widgets/scrolledmenuitems.hpp"
#include <cmath>

PromptMenuView::PromptMenuView()
{
}

void PromptMenuView::init(domain::Renderer* renderer,
                          KakouneContentView* kakoune_content_view)
{
    m_renderer = renderer;
    m_kakoune_content_view = kakoune_content_view;
    m_scrolled_menu_items = std::make_unique<ScrolledMenuItems>(MAX_VISIBLE_ITEMS);
    m_input = std::make_unique<Input>();
}

void PromptMenuView::render(const RenderContext& render_context, const domain::Menu &menu, int cursor_column)
{
    domain::Font* font = render_context.ui_options.font_menu;

    m_x = round((render_context.screen_width - WIDTH) / 2);
    m_height = 2 * BORDER_THICKNESS + 4 * SPACING_MEDIUM + m_input->height(font);

    float items_size = menu.hasItems() ? std::min(MAX_VISIBLE_ITEMS, (int)menu.getItems().items.size()) : 0;
    if (items_size > 0)
    {
        m_height += BORDER_THICKNESS + font->getLineHeight() * items_size;
    }

    LayoutManager layout(m_x, Y, WIDTH, m_height);

    m_renderer->renderRoundedRectWithShadow(
        getRGBAColor(render_context.ui_options.color_border, default_border_color), layout.current().x,
        layout.current().y, layout.current().width, layout.current().height, domain::CornerRadius(CORNER_RADIUS), SHADOW_LENGTH);

    layout.pad(BORDER_THICKNESS);

    m_renderer->renderRoundedRect(
        menu.getInputFace().getBg(render_context.default_face, render_context.ui_options.color_overrides), layout.current().x,
        layout.current().y, layout.current().width, 4 * SPACING_MEDIUM + m_input->height(font), domain::CornerRadius(CORNER_RADIUS, CORNER_RADIUS, items_size > 0 ? 0.0f : CORNER_RADIUS, items_size > 0 ? 0.0f : CORNER_RADIUS));

    layout.pad(SPACING_MEDIUM);

    LayoutManager input_layout = layout.sliceTop(SPACING_MEDIUM * 2 + m_input->height(font));
    input_layout.pad(SPACING_MEDIUM, 0);
    m_input->render(m_renderer, render_context, font, menu.getInput(), menu.getInputFace(), cursor_column, input_layout);

    if (menu.hasItems())
    {
        m_renderer->renderRect(getRGBAColor(render_context.ui_options.color_border, default_border_color), layout.current().x - SPACING_MEDIUM, layout.current().y, layout.current().width + SPACING_MEDIUM * 2, 1);

        layout.gapY(BORDER_THICKNESS);

        m_renderer->renderRoundedRect(
            menu.getItems().face.getBg(render_context.default_face, render_context.ui_options.color_overrides), layout.current().x - SPACING_MEDIUM,
            layout.current().y, layout.current().width + SPACING_MEDIUM*2, layout.current().height + SPACING_MEDIUM, domain::CornerRadius(0.0f, 0.0f, CORNER_RADIUS, CORNER_RADIUS));

        m_scrolled_menu_items->render(m_renderer, render_context, menu.getItems(), menu.getInputFace().getFg(render_context.default_face, render_context.ui_options.color_overrides), layout);
    }
}

float PromptMenuView::x() const {
    return m_x;
}

float PromptMenuView::y() const {
    return Y;
}

float PromptMenuView::width() const {
    return WIDTH;
}

float PromptMenuView::height() const {
    return m_height;
}

float PromptMenuView::scrolledItemsX() const {
    return m_scrolled_menu_items->x();
}

float PromptMenuView::scrolledItemsY() const {
    return m_scrolled_menu_items->y();
}

float PromptMenuView::scrolledItemsWidth() const {
    return m_scrolled_menu_items->width();
}

float PromptMenuView::scrolledItemsHeight() const {
    return m_scrolled_menu_items->height();
}

domain::MouseMoveResult PromptMenuView::onMouseMove(float x, float y, const domain::Menu& menu) {
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

std::optional<int> PromptMenuView::findItemAtPosition(float x, float y, const domain::Menu &menu) {
    if (!menu.hasItems()) return std::nullopt;
    return m_scrolled_menu_items->findItemAtPosition(x, y, menu.getItems());
}

void PromptMenuView::onMouseScroll(int scroll_amount, const domain::Menu &menu) {
    if (!menu.hasItems()) return;

    int total_items = menu.getItems().items.size();
    m_scrolled_menu_items->scroll(scroll_amount, total_items);
}

void PromptMenuView::ensureItemVisible(int index) {
    m_scrolled_menu_items->ensureItemVisible(index);
}
