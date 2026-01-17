#include "inlinemenu.hpp"
#include "domain/color.hpp"
#include "domain/glyphlinesbuilder.hpp"
#include "styling.hpp"
#include "widgets/scrolledmenuitems.hpp"

InlineMenuView::InlineMenuView()
{
}

void InlineMenuView::init(domain::Renderer* renderer,
                          KakouneContentView* kakoune_content_view)
{
    m_renderer = renderer;
    m_kakoune_content_view = kakoune_content_view;
    m_scrolled_menu_items = std::make_unique<ScrolledMenuItems>(MAX_VISIBLE_ITEMS);
}

float InlineMenuView::x() const {
    return m_x;
}

float InlineMenuView::y() const {
    return m_y;
}

float InlineMenuView::width() const {
    return m_width;
}

float InlineMenuView::height() const {
    return m_height;
}

void InlineMenuView::render(const RenderContext &render_context, const domain::Menu &menu)
{
    if (!menu.hasItems()) return;

    domain::Font* font = render_context.ui_options.font_menu;
    auto anchor = menu.getItems().anchor;

    auto menu_position = m_kakoune_content_view->coordToPixels(render_context.ui_options.font_content, anchor);

    auto menu_item_width = 0.0f;
    for (const auto& item : menu.getItems().items) {
        auto item_width = domain::GlyphLinesBuilder::build(item, font, render_context.font_manager).width();
        menu_item_width = std::max(menu_item_width, item_width);
    }
    m_width = std::min(menu_item_width + BORDER_THICKNESS * 2.0f + SPACING_MEDIUM * 2.0f + m_scrolled_menu_items->getRightPadding(menu.getItems().items.size()), std::min(MAX_MENU_WIDTH, render_context.screen_width));
    float items_size = std::min(MAX_VISIBLE_ITEMS, (int)menu.getItems().items.size());
    m_height = font->getLineHeight() * items_size + BORDER_THICKNESS * 2.0f;

    m_x = menu_position.first;
    if (m_x + m_width > render_context.screen_width)
    {
        m_x = render_context.screen_width - m_width;
    }

    m_y = menu_position.second + SPACING_SMALL + font->getLineHeight();
    if (m_y + m_height > render_context.screen_height)
    {
        m_y = menu_position.second - SPACING_SMALL - m_height;
    }

    LayoutManager layout(m_x, m_y, m_width, m_height);

    m_renderer->renderRect(getRGBAColor(render_context.ui_options.color_border, default_border_color),
                           layout.current().x, layout.current().y, layout.current().width, layout.current().height);

    layout.pad(BORDER_THICKNESS);

    m_renderer->renderRect(menu.getItems().face.getBg(render_context.default_face, render_context.ui_options.color_overrides),
                           layout.current().x, layout.current().y, layout.current().width, layout.current().height);

    layout.pad(0, SPACING_MEDIUM);

    m_scrolled_menu_items->render(m_renderer, render_context, menu.getItems(), menu.getItems().face.getFg(render_context.default_face, render_context.ui_options.color_overrides), layout);
}

float InlineMenuView::scrolledItemsX() const {
    return m_scrolled_menu_items->x();
}

float InlineMenuView::scrolledItemsY() const {
    return m_scrolled_menu_items->y();
}

float InlineMenuView::scrolledItemsWidth() const {
    return m_scrolled_menu_items->width();
}

float InlineMenuView::scrolledItemsHeight() const {
    return m_scrolled_menu_items->height();
}

domain::MouseMoveResult InlineMenuView::onMouseMove(float x, float y, const domain::Menu &menu) {
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

std::optional<int> InlineMenuView::findItemAtPosition(float x, float y, const domain::Menu &menu) {
    if (!menu.hasItems()) return std::nullopt;
    return m_scrolled_menu_items->findItemAtPosition(x, y, menu.getItems());
}

void InlineMenuView::onMouseScroll(int scroll_amount, const domain::Menu &menu) {
    if (!menu.hasItems()) return;

    int total_items = menu.getItems().items.size();
    m_scrolled_menu_items->scroll(scroll_amount, total_items);
}

void InlineMenuView::ensureItemVisible(int index) {
    m_scrolled_menu_items->ensureItemVisible(index);
}
