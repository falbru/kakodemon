#include "view/inlinemenu.hpp"
#include "view/styling.hpp"
#include "view/widgets/scrolledmenuitems.hpp"

InlineMenuView::InlineMenuView()
{
    m_font = std::make_shared<opengl::Font>("/usr/share/fonts/truetype/ubuntu/UbuntuMono-R.ttf", 17);
    m_scrolled_menu_items = std::make_unique<ScrolledMenuItems>(m_font, MAX_VISIBLE_ITEMS);
}

void InlineMenuView::init(std::shared_ptr<opengl::Renderer> renderer,
                          std::shared_ptr<KakouneContentView> kakoune_content_view)
{
    m_renderer = renderer;
    m_kakoune_content_view = kakoune_content_view;
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

void InlineMenuView::render(const KakouneClient &kakoune_client, float width, float height)
{
    auto anchor = kakoune_client.menu_anchor;

    auto menu_position = m_kakoune_content_view->coordToPixels(kakoune_client.menu_anchor);

    m_width = std::min(MAX_MENU_WIDTH, width);
    float items_size = std::min(MAX_VISIBLE_ITEMS, (int)kakoune_client.menu_items.size());
    m_height = m_font->getLineHeight() * items_size;

    m_x = menu_position.first;
    if (m_x + m_width > width)
    {
        m_x = width - m_width;
    }

    m_y = menu_position.second + SPACING_SMALL + m_font->getLineHeight();
    if (m_y + m_height > height)
    {
        m_y = menu_position.second - SPACING_SMALL - m_height;
    }

    LayoutManager layout(m_x, m_y, m_width, m_height);

    m_renderer->renderRect(kakoune_client.menu_face.bg.toCoreColor(kakoune_client.window_default_face.bg, false),
                           layout.current().x, layout.current().y, layout.current().width, layout.current().height);

    layout.pad(0, SPACING_MEDIUM);

    m_scrolled_menu_items->render(m_renderer, kakoune_client, layout);
}
