#include "promptmenu.hpp"
#include "domain/color.hpp"
#include "styling.hpp"
#include "widgets/scrolledmenuitems.hpp"

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

void PromptMenuView::render(domain::Font* font, domain::FontManager* font_manager, const KakouneClient &kakoune_client, float width, float height)
{
    if (!kakoune_client.state.menu.has_value())
        return;

    m_input->setPrompt(kakoune_client.state.menu->getInput().getPrompt());
    m_input->setContent(kakoune_client.state.menu->getInput().getContent());

    m_x = (width - WIDTH) / 2;
    m_height = 2 * BORDER_THICKNESS + 2 * SPACING_MEDIUM + m_input->height(font);

    float items_size = kakoune_client.state.menu->hasItems() ?
        std::min(MAX_VISIBLE_ITEMS, (int)kakoune_client.state.menu->getItems().items.size()) : 0;
    if (items_size > 0)
    {
        m_height += BORDER_THICKNESS + font->getLineHeight() * items_size;
    }

    LayoutManager layout(m_x, Y, WIDTH, m_height);

    float CORNER_RADIUS = 5.0f;
    float SHADOW_LENGTH = 15.0f;

    m_renderer->renderRoundedRectWithShadow(
        domain::RGBAColor{0.5, 0.5, 0.5, 1.0}, layout.current().x,
        layout.current().y, layout.current().width, layout.current().height, CORNER_RADIUS, SHADOW_LENGTH);

    layout.pad(BORDER_THICKNESS);

    m_renderer->renderRoundedRect(
        kakoune_client.state.mode_line.getDefaultFace().getBg(kakoune_client.state.default_face), layout.current().x,
        layout.current().y, layout.current().width, SPACING_MEDIUM + m_input->height(font) + SPACING_MEDIUM, CORNER_RADIUS, CORNER_RADIUS, items_size > 0 ? 0.0f : CORNER_RADIUS, items_size > 0 ? 0.0f : CORNER_RADIUS);

    layout.pad(SPACING_MEDIUM);

    m_input->render(m_renderer, font, font_manager, kakoune_client, layout);

    if (kakoune_client.state.menu->hasItems())
    {
        m_renderer->renderRect(domain::RGBAColor{0.5, 0.5, 0.5, 1.0}, layout.current().x - SPACING_MEDIUM, layout.current().y, layout.current().width + SPACING_MEDIUM * 2, 1);

        layout.gapY(BORDER_THICKNESS);

        m_renderer->renderRoundedRect(
            kakoune_client.state.menu->getItems().face.getBg(kakoune_client.state.default_face), layout.current().x - SPACING_MEDIUM,
            layout.current().y, layout.current().width + SPACING_MEDIUM*2, layout.current().height + SPACING_MEDIUM, 0.0f, 0.0f, CORNER_RADIUS, CORNER_RADIUS);

        m_scrolled_menu_items->render(m_renderer, font, font_manager, kakoune_client, layout);
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

domain::MouseMoveResult PromptMenuView::onMouseMove(float x, float y, const KakouneClient &kakoune_client) {
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

std::optional<int> PromptMenuView::findItemAtPosition(float x, float y, domain::Font *font, const KakouneClient &kakoune_client) {
    return m_scrolled_menu_items->findItemAtPosition(x, y, font, kakoune_client);
}

void PromptMenuView::onMouseScroll(int scroll_amount, const KakouneClient &kakoune_client) {
    if (!kakoune_client.state.menu.has_value() || !kakoune_client.state.menu->hasItems()) return;

    int total_items = kakoune_client.state.menu->getItems().items.size();
    m_scrolled_menu_items->scroll(scroll_amount, total_items);
}

void PromptMenuView::ensureItemVisible(int index) {
    m_scrolled_menu_items->ensureItemVisible(index);
}
