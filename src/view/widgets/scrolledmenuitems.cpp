#include "scrolledmenuitems.hpp"
#include "view/styling.hpp"
#include "view/glyphsequence.hpp"

ScrolledMenuItems::ScrolledMenuItems(int max_visible_items) : m_max_visible_items(max_visible_items) {
    m_scroll_bar = std::make_unique<ScrollBar>();
}

void ScrolledMenuItems::render(Renderer* renderer, Font* font, const KakouneClient &kakoune_client, LayoutManager &layout) {
    auto items_layout = layout.copy();
    items_layout.padRight(SPACING_MEDIUM + m_scroll_bar->width());

    int selected_index = kakoune_client.menu_selected_index;
    if (selected_index >= kakoune_client.menu_items.size())
        selected_index = -1;
    if (selected_index < m_scroll_offset)
    {
        m_scroll_offset = std::max(0, selected_index);
    }
    else if (selected_index >= m_scroll_offset + m_max_visible_items)
    {
        m_scroll_offset = std::max(0, selected_index - m_max_visible_items + 1);
    }

    for (int i = m_scroll_offset; i < m_scroll_offset + m_max_visible_items && i < kakoune_client.menu_items.size(); i++)
    {
        auto item = kakoune_client.menu_items.at(i);

        GlyphSequence item_value_glyphs = GlyphSequence(font, item.atoms[0].contents.trim(TrimDirection::Right));
        float item_secondary_width = 0;
        if (item.atoms.size() > 1)
        {
            item_secondary_width = font->width(item.atoms[1].contents.trim(TrimDirection::Left));
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
            renderer->renderRect(
                kakoune_client.menu_selected_face.bg.toCoreColor(kakoune_client.window_default_face.bg, false),
                items_layout.current().x - SPACING_MEDIUM, items_layout.current().y,
                items_layout.current().width + SPACING_MEDIUM * 3 + m_scroll_bar->width(), font->getLineHeight());
        }
        kakoune::Face item_face = i == selected_index ? kakoune_client.menu_selected_face : kakoune_client.menu_face;

        renderer->renderLine(font, item_value, item_face, items_layout.current().x, items_layout.current().y);
        renderer->renderLine(font, item_secondary, item_face,
                               items_layout.current().x + items_layout.current().width, items_layout.current().y,
                               core::Alignment::topRight());

        items_layout.sliceTop(font->getLineHeight());
    }

    if (kakoune_client.menu_items.size() > m_max_visible_items)
    {
        m_scroll_bar->setValue(m_scroll_offset, kakoune_client.menu_items.size() - m_max_visible_items, m_max_visible_items);
        m_scroll_bar->render(renderer, kakoune_client.status_default_face.fg.toCoreColor(std::nullopt, true), layout);
    }
}
