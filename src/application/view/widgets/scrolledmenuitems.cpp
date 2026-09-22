#include "scrolledmenuitems.hpp"
#include "../styling.hpp"
#include "domain/atom.hpp"
#include "domain/codepointstring.hpp"
#include "domain/glyphatom.hpp"
#include "domain/glyphline.hpp"
#include "domain/glyphlinesbuilder.hpp"
#include <optional>

ScrolledMenuItems::ScrolledMenuItems(int max_visible_items) : m_max_visible_items(max_visible_items)
{
    m_scroll_bar = std::make_unique<ScrollBar>();
}

void ScrolledMenuItems::render(domain::Renderer *renderer, const RenderContext &render_context, MenuViewState &state,
                               const domain::MenuItems &menu_items, const domain::RGBAColor &scrollbar_color,
                               LayoutManager &layout)
{
    domain::Font *font = render_context.ui_options.font_menu;
    auto items_layout = layout.copy();
    if (menu_items.items.size() > m_max_visible_items)
    {
        items_layout.padRight(SPACING_MEDIUM + m_scroll_bar->width());
    }

    m_x = items_layout.current().x;
    m_y = items_layout.current().y;

    int selected_index = menu_items.selected_index;

    if (state.items_scroll_offset + m_max_visible_items > menu_items.items.size())
    {
        state.items_scroll_offset = std::max(0, (int)menu_items.items.size() - m_max_visible_items);
    }

    for (int i = state.items_scroll_offset;
         i < state.items_scroll_offset + m_max_visible_items && i < menu_items.items.size(); i++)
    {
        auto &item = menu_items.items.at(i);

        if (item.getAtoms().size() == 0)
        {
            continue;
        }

        auto item_left = domain::GlyphLinesBuilder::build(domain::Line({item.at(0)}).trim(domain::TrimDirection::Right),
                                                          font, render_context.font_manager);

        std::optional<domain::GlyphLine> item_right =
            item.getAtoms().size() > 1
                ? std::make_optional(domain::GlyphLinesBuilder::build(
                      domain::Line({item.at(item.getAtoms().size() - 1)}).trim(domain::TrimDirection::Left), font,
                      render_context.font_manager))
                : std::nullopt;

        float item_right_width = item_right.has_value() ? item_right.value().width() : 0;

        if (item_left.width() + item_right_width > items_layout.current().width)
        {
            item_left.truncate(items_layout.current().width - item_right_width, font, render_context.font_manager);
        }

        if (i == selected_index)
        {
            renderer->renderRect(
                menu_items.selected_face.getBg(render_context.default_face, render_context.ui_options.color_overrides),
                items_layout.current().x - SPACING_MEDIUM, items_layout.current().y,
                items_layout.current().width + SPACING_MEDIUM * 2 + getRightPadding(menu_items.items.size()),
                font->getLineHeight());
        }
        domain::Face item_face = i == selected_index ? menu_items.selected_face : menu_items.face;

        renderer->renderLine(render_context.textConfig(font), domain::GlyphLine({item_left}).toLine(), item_face,
                             items_layout.current().x, items_layout.current().y);
        if (item_right.has_value())
        {
            renderer->renderLine(render_context.textConfig(font), domain::GlyphLine({item_right.value()}).toLine(),
                                 item_face, items_layout.current().x + items_layout.current().width,
                                 items_layout.current().y, domain::Alignment::topRight());
        }

        items_layout.sliceTop(font->getLineHeight());
    }

    if (menu_items.items.size() > m_max_visible_items)
    {
        m_scroll_bar->setValue(state.items_scroll_offset, menu_items.items.size() - m_max_visible_items,
                               m_max_visible_items);
        m_scroll_bar->render(renderer, scrollbar_color, layout);
    }

    int visible_items = std::min(m_max_visible_items, (int)menu_items.items.size());

    m_width = items_layout.current().width;
    m_height = visible_items * font->getLineHeight();
}

float ScrolledMenuItems::width() const
{
    return m_width;
}

float ScrolledMenuItems::height() const
{
    return m_height;
}

float ScrolledMenuItems::x() const
{
    return m_x;
}

float ScrolledMenuItems::y() const
{
    return m_y;
}

std::optional<int> ScrolledMenuItems::findItemAtPosition(float x, float y, const MenuViewState &state,
                                                         const domain::MenuItems &menu_items)
{
    if (x < m_x || x >= m_x + m_width || y < m_y || y >= m_y + m_height)
    {
        return std::nullopt;
    }

    float relative_y = y - m_y;
    float line_height = (m_height / std::min(m_max_visible_items, (int)menu_items.items.size()));
    int clicked_item_offset = static_cast<int>(relative_y / line_height);
    int clicked_item_index = state.items_scroll_offset + clicked_item_offset;

    if (clicked_item_index >= 0 && clicked_item_index < menu_items.items.size())
    {
        return clicked_item_index;
    }

    return std::nullopt;
}

void ScrolledMenuItems::scroll(MenuViewState &state, int amount, int total_items)
{
    state.items_scroll_offset += amount;
    int max_scroll = std::max(0, total_items - m_max_visible_items);
    state.items_scroll_offset = std::max(0, std::min(max_scroll, state.items_scroll_offset));
}

void ScrolledMenuItems::ensureItemVisible(MenuViewState &state, int index)
{
    if (index < state.items_scroll_offset)
    {
        state.items_scroll_offset = std::max(0, index);
    }
    else if (index >= state.items_scroll_offset + m_max_visible_items)
    {
        state.items_scroll_offset = std::max(0, index - m_max_visible_items + 1);
    }
}

float ScrolledMenuItems::getRightPadding(int total_items) const
{
    if (total_items > m_max_visible_items)
    {
        return SPACING_MEDIUM + m_scroll_bar->width();
    }
    return 0.0f;
}
