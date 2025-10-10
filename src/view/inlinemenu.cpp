#include "core/alignment.hpp"
#include "core/utf8string.hpp"
#include "spdlog/spdlog.h"
#include "view/glyphsequence.hpp"
#include "view/inlinemenu.hpp"
#include "view/styling.hpp"

InlineMenuView::InlineMenuView()
{
    m_font = std::make_shared<opengl::Font>("/home/falk/.fonts/MonoLisa/ttf/MonoLisa-Regular.ttf", 14);
    m_scroll_bar = std::make_unique<ScrollBar>();
}

void InlineMenuView::init(std::shared_ptr<opengl::Renderer> renderer,
                          std::shared_ptr<KakouneContentView> kakoune_content_view)
{
    m_renderer = renderer;
    m_kakoune_content_view = kakoune_content_view;
}

void InlineMenuView::render(const KakouneClient &kakoune_client, float width, float height)
{
    auto anchor = kakoune_client.menu_anchor;

    auto menu_position = m_kakoune_content_view->coordToPixels(kakoune_client.menu_anchor);
    float menu_width = std::min(MAX_MENU_WIDTH, width);
    float items_size = std::min(MAX_VISIBLE_ITEMS, (int)kakoune_client.menu_items.size());
    float menu_height = m_font->getLineHeight() * items_size;

    float menu_x = menu_position.first;
    if (menu_x + menu_width > width) {
        menu_x = width - menu_width;
    }

    float menu_y = menu_position.second + SPACING_SMALL + m_font->getLineHeight();
    if (menu_y + menu_height > height)
    {
        menu_y = menu_position.second - SPACING_SMALL - menu_height;
    }

    LayoutManager layout(menu_x, menu_y, menu_width, menu_height);

    m_renderer->renderRect(kakoune_client.menu_face.bg.toCoreColor(kakoune_client.window_default_face.bg, false),
                           layout.current().x, layout.current().y, layout.current().width, layout.current().height);

    layout.pad(0, SPACING_MEDIUM);

    renderScrolledContent(kakoune_client, layout, MAX_VISIBLE_ITEMS);
}

void InlineMenuView::renderScrolledContent(const KakouneClient &kakoune_client, LayoutManager &layout, int max_items)
{
    auto items_layout = layout.copy();
    items_layout.padRight(SPACING_MEDIUM + m_scroll_bar->width());

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

        GlyphSequence item_value_glyphs = GlyphSequence(m_font, item.atoms[0].contents.trim(TrimDirection::Right));
        float item_secondary_width = 0;
        if (item.atoms.size() > 1)
        {
            item_secondary_width = GlyphSequence(m_font, item.atoms[1].contents.trim(TrimDirection::Left)).width();
        }

        if (item_value_glyphs.width() + item_secondary_width > items_layout.current().width)
        {
            item_value_glyphs.truncate(items_layout.current().width - item_secondary_width);
        }

        auto item_value = kakoune::Line{{item.atoms[0]}};
        auto item_secondary = item.atoms.size() > 1 ? kakoune::Line{{item.atoms[1]}} : kakoune::Line();
        item_value.atoms[0].contents = item_value_glyphs.toUTF8String();

        if (i == selected_index)
        {
            m_renderer->renderRect(
                kakoune_client.menu_selected_face.bg.toCoreColor(kakoune_client.window_default_face.bg, false),
                items_layout.current().x - SPACING_MEDIUM, items_layout.current().y,
                items_layout.current().width + SPACING_MEDIUM * 3 + m_scroll_bar->width(), line_height);
        }
        m_renderer->renderLine(*m_font, item_value,
                               i == selected_index ? kakoune_client.menu_selected_face : kakoune_client.menu_face,
                               items_layout.current().x, items_layout.current().y);
        m_renderer->renderLine(*m_font, item_secondary,
                               i == selected_index ? kakoune_client.menu_selected_face : kakoune_client.menu_face,
                               items_layout.current().x + items_layout.current().width, items_layout.current().y,
                               core::Alignment::topRight());

        items_layout.sliceTop(line_height);
    }

    if (kakoune_client.menu_items.size() > max_items)
    {
        m_scroll_bar->setValue(m_scroll_offset, kakoune_client.menu_items.size() - max_items, max_items);
        m_scroll_bar->render(m_renderer, kakoune_client.status_default_face.fg.toCoreColor(std::nullopt, true), layout);
    }
}
