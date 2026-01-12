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

void InlineMenuView::render(const domain::UIOptions &ui_options, domain::FontManager* font_manager, const KakouneClient &kakoune_client, float width, float height)
{
    if (!kakoune_client.state.menu.has_value() || !kakoune_client.state.menu->hasItems()) return;

    auto anchor = kakoune_client.state.menu->getItems().anchor;

    auto menu_position = m_kakoune_content_view->coordToPixels(ui_options.font, anchor);

    auto menu_item_width = domain::GlyphLinesBuilder::build(kakoune_client.state.menu->getItems().items.at(0), ui_options.font, font_manager).width(); // For the menu, all lines will have the same length

    m_width = std::min(menu_item_width, std::min(MAX_MENU_WIDTH, width));
    float items_size = std::min(MAX_VISIBLE_ITEMS, (int)kakoune_client.state.menu->getItems().items.size());
    m_height = ui_options.font->getLineHeight() * items_size + BORDER_THICKNESS * 2.0f;

    m_x = menu_position.first;
    if (m_x + m_width > width)
    {
        m_x = width - m_width;
    }

    m_y = menu_position.second + SPACING_SMALL + ui_options.font->getLineHeight();
    if (m_y + m_height > height)
    {
        m_y = menu_position.second - SPACING_SMALL - m_height;
    }

    LayoutManager layout(m_x, m_y, m_width, m_height);

    m_renderer->renderRect(domain::RGBAColor{0.5, 0.5, 0.5, 1},
                           layout.current().x, layout.current().y, layout.current().width, layout.current().height);

    layout.pad(BORDER_THICKNESS);

    m_renderer->renderRect(kakoune_client.state.menu->getItems().face.getBg(kakoune_client.state.default_face, ui_options.color_overrides),
                           layout.current().x, layout.current().y, layout.current().width, layout.current().height);

    layout.pad(0, SPACING_MEDIUM);

    m_scrolled_menu_items->render(m_renderer, ui_options, font_manager, kakoune_client, layout);
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

domain::MouseMoveResult InlineMenuView::onMouseMove(float x, float y, const KakouneClient &kakoune_client) {
    if (kakoune_client.state.menu.has_value() && kakoune_client.state.menu->hasItems()) {
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

std::optional<int> InlineMenuView::findItemAtPosition(float x, float y, domain::Font *font, const KakouneClient &kakoune_client) {
    return m_scrolled_menu_items->findItemAtPosition(x, y, font, kakoune_client);
}

void InlineMenuView::onMouseScroll(int scroll_amount, const KakouneClient &kakoune_client) {
    if (!kakoune_client.state.menu.has_value() || !kakoune_client.state.menu->hasItems()) return;

    int total_items = kakoune_client.state.menu->getItems().items.size();
    m_scrolled_menu_items->scroll(scroll_amount, total_items);
}

void InlineMenuView::ensureItemVisible(int index) {
    m_scrolled_menu_items->ensureItemVisible(index);
}
