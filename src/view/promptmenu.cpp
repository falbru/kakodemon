#include "view/promptmenu.hpp"
#include "view/styling.hpp"
#include "view/widgets/scrolledmenuitems.hpp"

PromptMenuView::PromptMenuView()
{
}

void PromptMenuView::init(Renderer* renderer,
                          KakouneContentView* kakoune_content_view)
{
    m_renderer = renderer;
    m_kakoune_content_view = kakoune_content_view;
}

void PromptMenuView::render(Font* font, const KakouneClient &kakoune_client, float width, float height)
{
    if (!m_input) {
        m_input = std::make_unique<Input>(font);
    }
    if (!m_scrolled_menu_items) {
        m_scrolled_menu_items = std::make_unique<ScrolledMenuItems>(MAX_VISIBLE_ITEMS);
    }

    if (kakoune_client.status_line.prompt.atoms.size() < 1 && kakoune_client.status_line.content.atoms.size() < 1)
        return;

    static bool opened_before = false; // TODO remove this???
    if (!kakoune_client.menu_visible && !opened_before)
        return;
    opened_before = true;

    m_input->setPrompt(kakoune_client.status_line.prompt);
    m_input->setContent(kakoune_client.status_line.content);

    m_x = (width - WIDTH) / 2;
    m_height = 2 * SPACING_MEDIUM + m_input->height();

    float items_size = std::min(MAX_VISIBLE_ITEMS, (int)kakoune_client.menu_items.size());
    if (kakoune_client.menu_visible)
    {
        m_height += SPACING_MEDIUM + font->getLineHeight() * items_size;
    }

    LayoutManager layout(m_x, Y, WIDTH, m_height);

    m_renderer->renderRectWithShadow(
        kakoune_client.menu_face.bg.toCoreColor(kakoune_client.window_default_face.bg, false), layout.current().x,
        layout.current().y, layout.current().width, layout.current().height, 15.0f);

    layout.pad(SPACING_MEDIUM);

    m_input->render(m_renderer, kakoune_client, layout);

    if (kakoune_client.menu_visible)
    {
        layout.gapY(SPACING_MEDIUM);
        m_scrolled_menu_items->render(m_renderer, font, kakoune_client, layout);
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
