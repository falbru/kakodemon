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

void SearchMenuView::render(domain::Font *font, domain::FontManager *font_manager, const KakouneClient &kakoune_client, float width, float height) {
    if (!kakoune_client.state.menu.has_value())
        return;

    m_input->setPrompt(kakoune_client.state.menu->getInput().getPrompt());
    m_input->setContent(kakoune_client.state.menu->getInput().getContent());

    m_x = width - WIDTH;
    m_height = 2 * SPACING_MEDIUM + m_input->height(font);

    float items_size = kakoune_client.state.menu->hasItems() ?
        std::min(MAX_VISIBLE_ITEMS, (int)kakoune_client.state.menu->getItems().items.size()) : 0;
    if (items_size > 0)
    {
        m_height += SPACING_MEDIUM + font->getLineHeight() * items_size;
    }

    LayoutManager layout(m_x, 0, WIDTH, m_height);

    domain::Face bg_face = kakoune_client.state.menu->hasItems() ?
        kakoune_client.state.menu->getItems().face :
        kakoune_client.state.default_face;
    m_renderer->renderRoundedRect(
        bg_face.getBg(kakoune_client.state.default_face), layout.current().x,
        layout.current().y, layout.current().width, layout.current().height, 0.0f, 0.0f, 10.0f, 10.0f);

    layout.pad(SPACING_MEDIUM);

    m_input->render(m_renderer, font, font_manager, kakoune_client, layout);

    if (kakoune_client.state.menu->hasItems())
    {
        layout.gapY(SPACING_MEDIUM);
        m_scrolled_menu_items->render(m_renderer, font, font_manager, kakoune_client, layout);
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

domain::MouseMoveResult SearchMenuView::onMouseMove(float x, float y, const KakouneClient &kakoune_client) {
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

std::optional<int> SearchMenuView::findItemAtPosition(float x, float y, domain::Font *font, const KakouneClient &kakoune_client) {
    return m_scrolled_menu_items->findItemAtPosition(x, y, font, kakoune_client);
}

void SearchMenuView::onMouseScroll(int scroll_amount, const KakouneClient &kakoune_client) {
    if (!kakoune_client.state.menu.has_value() || !kakoune_client.state.menu->hasItems()) return;

    int total_items = kakoune_client.state.menu->getItems().items.size();
    m_scrolled_menu_items->scroll(scroll_amount, total_items);
}

void SearchMenuView::ensureSelectedItemVisible(int selected_index) {
    m_scrolled_menu_items->ensureSelectedItemVisible(selected_index);
}
