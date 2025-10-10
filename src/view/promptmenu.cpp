#include "view/glyphsequence.hpp"
#include "view/promptmenu.hpp"
#include "view/styling.hpp"

PromptMenuView::PromptMenuView()
{
    m_font = std::make_shared<opengl::Font>("/home/falk/.fonts/MonoLisa/ttf/MonoLisa-Regular.ttf", 14);
    m_input = std::make_unique<Input>(m_font);
    m_scroll_bar = std::make_unique<ScrollBar>();
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
        renderScrolledContent(kakoune_client, layout, MAX_VISIBLE_ITEMS);
    }
}

void PromptMenuView::renderScrolledContent(const KakouneClient &kakoune_client, LayoutManager &layout, int max_items)
{
    auto items_layout = layout.copy();
    float line_height = m_font->getLineHeight();

    int selected_index = kakoune_client.menu_selected_index;
    if (selected_index >= kakoune_client.menu_items.size())
        selected_index = -1;
    if (selected_index < m_scroll_offset)
    {
        m_scroll_offset = std::max(0, selected_index);
    }
    else if (selected_index >= m_scroll_offset + max_items)
    {
        m_scroll_offset = std::max(0, selected_index - max_items + 1);
    }

    for (int i = m_scroll_offset; i < m_scroll_offset + max_items && i < kakoune_client.menu_items.size(); i++)
    {
        auto item = kakoune_client.menu_items.at(i);

        GlyphSequence glyphs = GlyphSequence(m_font, item.atoms[0].contents);
        if (glyphs.width() > items_layout.current().width)
        {
            glyphs.truncate(items_layout.current().width);
            item.atoms[0].contents = glyphs.toUTF8String();
        }

        if (i == selected_index)
        {
            m_renderer->renderRect(
                kakoune_client.menu_selected_face.bg.toCoreColor(kakoune_client.window_default_face.bg, false),
                items_layout.current().x - SPACING_MEDIUM, items_layout.current().y,
                items_layout.current().width + SPACING_MEDIUM * 2, line_height);
        }

        m_renderer->renderLine(m_font, item,
                               i == selected_index ? kakoune_client.menu_selected_face : kakoune_client.menu_face,
                               items_layout.current().x, items_layout.current().y);

        items_layout.sliceTop(line_height);
    }

    if (kakoune_client.menu_items.size() > max_items)
    {
        m_scroll_bar->setValue(m_scroll_offset, kakoune_client.menu_items.size() - max_items, max_items);
        m_scroll_bar->render(m_renderer, kakoune_client.status_default_face.fg.toCoreColor(std::nullopt, true), layout);
    }
}
