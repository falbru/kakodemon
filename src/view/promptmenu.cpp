#include "view/promptmenu.hpp"
#include "view/styling.hpp"
#include "view/widgets/scrolledmenuitems.hpp"

PromptMenuView::PromptMenuView()
{
    m_font = std::make_shared<opengl::Font>("/home/falk/.fonts/MonoLisa/ttf/MonoLisa-Regular.ttf", 14);
    m_input = std::make_unique<Input>(m_font);
    m_scrolled_menu_items = std::make_unique<ScrolledMenuItems>(m_font, MAX_VISIBLE_ITEMS);
}

void PromptMenuView::init(std::shared_ptr<opengl::Renderer> renderer,
                          std::shared_ptr<KakouneContentView> kakoune_content_view)
{
    m_renderer = renderer;
    m_kakoune_content_view = kakoune_content_view;
}

void PromptMenuView::render(const KakouneClient &kakoune_client, float width, float height)
{
    if (kakoune_client.status_line.atoms.size() < 1)
        return;

    static bool opened_before = false; // TODO remove this???
    if (!kakoune_client.menu_visible && !opened_before)
        return;
    opened_before = true;

    m_input->setContent(kakoune_client.status_line);

    float menu_x = (width - WIDTH) / 2;
    float menu_height = 2 * SPACING_MEDIUM + m_input->height();

    float items_size = std::min(MAX_VISIBLE_ITEMS, (int)kakoune_client.menu_items.size());
    if (kakoune_client.menu_visible)
    {
        menu_height += SPACING_MEDIUM + m_font->getLineHeight() * items_size;
    }

    LayoutManager layout(menu_x, Y, WIDTH, menu_height);

    m_renderer->renderRectWithShadow(
        kakoune_client.menu_face.bg.toCoreColor(kakoune_client.window_default_face.bg, false), layout.current().x,
        layout.current().y, layout.current().width, layout.current().height, 15.0f);

    layout.pad(SPACING_MEDIUM);

    m_input->render(m_renderer, kakoune_client, layout);

    if (kakoune_client.menu_visible)
    {
        layout.gapY(SPACING_MEDIUM);
        m_scrolled_menu_items->render(m_renderer, kakoune_client, layout);
    }
}
