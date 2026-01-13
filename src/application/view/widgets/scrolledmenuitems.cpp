#include "scrolledmenuitems.hpp"
#include "application/view/glyphsequence.hpp"
#include "../styling.hpp"

ScrolledMenuItems::ScrolledMenuItems(int max_visible_items) : m_max_visible_items(max_visible_items) {
    m_scroll_bar = std::make_unique<ScrollBar>();
}

void ScrolledMenuItems::render(domain::Renderer* renderer, const RenderContext &render_context, const domain::MenuItems &menu_items, const domain::RGBAColor &scrollbar_color, LayoutManager &layout) {
    domain::Font* font = render_context.ui_options.font_menu;
    auto items_layout = layout.copy();
    if (menu_items.items.size() > m_max_visible_items) {
        items_layout.padRight(SPACING_MEDIUM + m_scroll_bar->width());
    }

    m_x = items_layout.current().x;
    m_y = items_layout.current().y;

    int selected_index = menu_items.selected_index;

    for (int i = m_scroll_offset; i < m_scroll_offset + m_max_visible_items && i < menu_items.items.size(); i++)
    {
        auto item = menu_items.items.at(i);

        GlyphSequence item_value_glyphs = GlyphSequence(font, render_context.font_manager, item.at(0).getContents().trim(domain::TrimDirection::Right));
        float item_secondary_width = 0;
        if (item.getAtoms().size() > 1)
        {
            item_secondary_width = GlyphSequence(font, render_context.font_manager, item.at(1).getContents().trim(domain::TrimDirection::Left)).width();
        }

        if (item_value_glyphs.width() + item_secondary_width > items_layout.current().width)
        {
            item_value_glyphs.truncate(items_layout.current().width - item_secondary_width);
        }

        auto item_value = domain::Line{{domain::Atom(item_value_glyphs.toUTF8String(), item.at(0).getFace())}};
        auto item_secondary = item.size() > 1 ? domain::Line{{item.at(1)}} : domain::Line();

        if (i == selected_index)
        {
            renderer->renderRect(
                menu_items.selected_face.getBg(render_context.default_face, render_context.ui_options.color_overrides),
                items_layout.current().x - SPACING_MEDIUM, items_layout.current().y,
                items_layout.current().width + SPACING_MEDIUM * 3 + m_scroll_bar->width(), font->getLineHeight());
        }
        domain::Face item_face = i == selected_index ? menu_items.selected_face : menu_items.face;

        renderer->renderLine(render_context.textConfig(font), item_value, item_face, items_layout.current().x, items_layout.current().y);
        renderer->renderLine(render_context.textConfig(font), item_secondary, item_face,
                               items_layout.current().x + items_layout.current().width, items_layout.current().y,
                               domain::Alignment::topRight());

        items_layout.sliceTop(font->getLineHeight());
    }

    if (menu_items.items.size() > m_max_visible_items)
    {
        m_scroll_bar->setValue(m_scroll_offset, menu_items.items.size() - m_max_visible_items, m_max_visible_items);
        m_scroll_bar->render(renderer, scrollbar_color, layout);
    }

    int visible_items = std::min(m_max_visible_items, (int)menu_items.items.size());
    m_width = items_layout.current().width;
    m_height = visible_items * font->getLineHeight();
}

float ScrolledMenuItems::width() const {
    return m_width;
}

float ScrolledMenuItems::height() const {
    return m_height;
}

float ScrolledMenuItems::x() const {
    return m_x;
}

float ScrolledMenuItems::y() const {
    return m_y;
}

std::optional<int> ScrolledMenuItems::findItemAtPosition(float x, float y, const domain::MenuItems &menu_items) {
    if (x < m_x || x >= m_x + m_width || y < m_y || y >= m_y + m_height) {
        return std::nullopt;
    }

    float relative_y = y - m_y;
    float line_height = (m_height / std::min(m_max_visible_items, (int)menu_items.items.size()));
    int clicked_item_offset = static_cast<int>(relative_y / line_height);
    int clicked_item_index = m_scroll_offset + clicked_item_offset;

    if (clicked_item_index >= 0 && clicked_item_index < menu_items.items.size()) {
        return clicked_item_index;
    }

    return std::nullopt;
}

void ScrolledMenuItems::scroll(int amount, int total_items) {
    m_scroll_offset += amount;
    int max_scroll = std::max(0, total_items - m_max_visible_items);
    m_scroll_offset = std::max(0.0f, std::min((float)max_scroll, m_scroll_offset));
}

void ScrolledMenuItems::ensureItemVisible(int index) {
    if (index < m_scroll_offset)
    {
        m_scroll_offset = std::max(0, index);
    }
    else if (index >= m_scroll_offset + m_max_visible_items)
    {
        m_scroll_offset = std::max(0, index - m_max_visible_items + 1);
    }
}

float ScrolledMenuItems::getRightPadding(int total_items) const {
    if (total_items > m_max_visible_items) {
        return SPACING_MEDIUM + m_scroll_bar->width();
    }
    return 0.0f;
}
