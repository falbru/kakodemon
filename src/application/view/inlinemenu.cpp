#include "inlinemenu.hpp"
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

void InlineMenuView::render(domain::Font* font, const KakouneClient &kakoune_client, float width, float height)
{
    auto anchor = kakoune_client.menu_anchor;

    auto menu_position = m_kakoune_content_view->coordToPixels(font, kakoune_client.menu_anchor);

    m_width = std::min(MAX_MENU_WIDTH, width);
    float items_size = std::min(MAX_VISIBLE_ITEMS, (int)kakoune_client.menu_items.size());
    m_height = font->getLineHeight() * items_size;

    m_x = menu_position.first;
    if (m_x + m_width > width)
    {
        m_x = width - m_width;
    }

    m_y = menu_position.second + SPACING_SMALL + font->getLineHeight();
    if (m_y + m_height > height)
    {
        m_y = menu_position.second - SPACING_SMALL - m_height;
    }

    LayoutManager layout(m_x, m_y, m_width, m_height);

    m_renderer->renderRect(kakoune_client.menu_face.bg.toCoreColor(kakoune_client.window_default_face.bg, false),
                           layout.current().x, layout.current().y, layout.current().width, layout.current().height);

    layout.pad(0, SPACING_MEDIUM);

    m_scrolled_menu_items->render(m_renderer, font, kakoune_client, layout);
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

domain::MouseMoveResult InlineMenuView::onMouseMove(float x, float y) {
    float items_x = m_scrolled_menu_items->x();
    float items_y = m_scrolled_menu_items->y();
    float items_width = m_scrolled_menu_items->width();
    float items_height = m_scrolled_menu_items->height();

    if (x >= items_x && x < items_x + items_width && y >= items_y && y < items_y + items_height) {
        return domain::MouseMoveResult{domain::Cursor::POINTER};
    }

    if (x >= this->x() && x < this->x() + width() && y >= this->y() && y < this->y() + height()) {
        return domain::MouseMoveResult{domain::Cursor::DEFAULT};
    }

    return domain::MouseMoveResult{std::nullopt};
}
