#include "application/controller/menucontroller.hpp"
#include "application/model/kakouneclient.hpp"
#include "domain/alignment.hpp"
#include "domain/color.hpp"
#include "domain/geometry.hpp"
#include "domain/glyphlinesbuilder.hpp"
#include "domain/infobox.hpp"
#include "domain/lines.hpp"
#include "infobox.hpp"
#include "application/view/layoutmanager.hpp"
#include "application/view/styling.hpp"
#include <optional>
#include <variant>
#include <spdlog/spdlog.h>

InfoBoxView::InfoBoxView()
{
    m_scroll_bar = std::make_unique<ScrollBar>();
}

void InfoBoxView::init(domain::Renderer* renderer, MenuController* menu_controller,
                       KakouneContentView* kakoune_content_view)
{
    m_renderer = renderer;
    m_menu_controller = menu_controller;
    m_kakoune_content_view = kakoune_content_view;
}

std::optional<Placement> InfoBoxView::tryPlaceInfoBox(PlacementDirection direction, CrossAxisAlignment alignment,
                                                      const domain::Lines &content,
                                                      const domain::Rectangle &anchor, float layout_width, float layout_height, domain::Font* font, domain::FontManager* font_manager, const domain::Rectangle& menu_rectangle, const domain::CursorPosition &cursor_position)
{
    domain::GlyphLines glyph_lines = domain::GlyphLinesBuilder::build(content, font, font_manager);

    domain::Rectangle cursor_rect;
    bool has_cursor = false;

    if (std::holds_alternative<domain::BufferContentPosition>(cursor_position)) {
        auto buffer_pos = std::get<domain::BufferContentPosition>(cursor_position);
        auto cursor_pixel_pos = m_kakoune_content_view->coordToPixels(font, buffer_pos.coord);
        cursor_rect.x = cursor_pixel_pos.first;
        cursor_rect.y = cursor_pixel_pos.second;
        cursor_rect.width = m_kakoune_content_view->getCellWidth(font);
        cursor_rect.height = m_kakoune_content_view->getCellHeight(font);
        has_cursor = true;
    }

    domain::Rectangle info_box;

    info_box.width = glyph_lines.width();
    info_box.height = glyph_lines.height();

    if (info_box.width > MAX_WIDTH - SPACING_MEDIUM * 2.0f)
    {
        glyph_lines.wrap(MAX_WIDTH - SPACING_MEDIUM * 2.0f);
        info_box.width = glyph_lines.width();
        info_box.height = glyph_lines.height();
    }

    float scrollbar_space = SPACING_MEDIUM + m_scroll_bar->width();

    switch (direction)
    {
    case PlacementDirection::RIGHT:
    case PlacementDirection::RIGHT_WRAPPED: {
        info_box.x = anchor.x + anchor.width;

        if (direction == PlacementDirection::RIGHT_WRAPPED)
        {
            float available_width = m_kakoune_content_view->width() - info_box.x - SPACING_MEDIUM * 2.0f;

            if (available_width < MIN_WIDTH)
                return std::nullopt;

            glyph_lines.wrap(available_width);
            info_box.width = glyph_lines.width();
            info_box.height = glyph_lines.height();

            if (info_box.height > MAX_HEIGHT)
            {
                glyph_lines.wrap(available_width - scrollbar_space);
                info_box.width = glyph_lines.width() + scrollbar_space;
                info_box.height = glyph_lines.height();
            }
        }
        else if (info_box.x + info_box.width + SPACING_MEDIUM * 2.0f > layout_width)
        {
            return std::nullopt;
        }

        if (info_box.height > MAX_HEIGHT)
        {
            if (direction != PlacementDirection::RIGHT_WRAPPED)
            {
                float wrap_width = info_box.width - scrollbar_space;
                glyph_lines.wrap(wrap_width);
                info_box.width = glyph_lines.width() + scrollbar_space;
                info_box.height = glyph_lines.height();
            }
            info_box.height = MAX_HEIGHT;
        }

        switch(alignment) {
            case CrossAxisAlignment::START:
                info_box.y = anchor.y;
                break;
            case CrossAxisAlignment::CENTER:
                info_box.y = anchor.y + (anchor.height - info_box.height) / 2.0f;
                break;
            case CrossAxisAlignment::END:
                info_box.y = anchor.y + anchor.height - info_box.height;
                break;
        }

        if (info_box.y + info_box.height + SPACING_MEDIUM * 2.0f > m_kakoune_content_view->height())
        {
            info_box.y = std::max(0.0f, m_kakoune_content_view->height() - info_box.height - SPACING_MEDIUM * 2.0f);
        }

        if (info_box.intersects(menu_rectangle)) {
            return std::nullopt;
        }

        if (has_cursor && info_box.intersects(cursor_rect)) {
            return std::nullopt;
        }

        return Placement{glyph_lines.toLines(), info_box};
    }
    case PlacementDirection::LEFT:
    case PlacementDirection::LEFT_WRAPPED: {
        if (direction == PlacementDirection::LEFT_WRAPPED)
        {
            float available_width = anchor.x - SPACING_MEDIUM * 2.0f;
            if (available_width < MIN_WIDTH)
                return std::nullopt;

            glyph_lines.wrap(available_width);
            info_box.width = glyph_lines.width();
            info_box.height = glyph_lines.height();

            if (info_box.height > MAX_HEIGHT)
            {
                glyph_lines.wrap(available_width - scrollbar_space);
                info_box.width = glyph_lines.width() + scrollbar_space;
                info_box.height = glyph_lines.height();
            }
        }

        if (info_box.height > MAX_HEIGHT)
        {
            if (direction != PlacementDirection::LEFT_WRAPPED)
            {
                float wrap_width = info_box.width - scrollbar_space;
                glyph_lines.wrap(wrap_width);
                info_box.width = glyph_lines.width() + scrollbar_space;
                info_box.height = glyph_lines.height();
            }
            info_box.height = MAX_HEIGHT;
        }

        info_box.x = anchor.x - info_box.width - SPACING_MEDIUM * 2.0f;

        if (info_box.x < 0)
        {
            return std::nullopt;
        }

        switch(alignment) {
            case CrossAxisAlignment::START:
                info_box.y = anchor.y;
                break;
            case CrossAxisAlignment::CENTER:
                info_box.y = anchor.y + (anchor.height - info_box.height) / 2.0f;
                break;
            case CrossAxisAlignment::END:
                info_box.y = anchor.y + anchor.height - info_box.height;
                break;
        }

        if (info_box.y + info_box.height + SPACING_MEDIUM * 2.0f > m_kakoune_content_view->height())
        {
            info_box.y = std::max(0.0f, m_kakoune_content_view->height() - info_box.height - SPACING_MEDIUM * 2.0f);
        }

        if (info_box.intersects(menu_rectangle)) {
            return std::nullopt;
        }

        if (has_cursor && info_box.intersects(cursor_rect)) {
            return std::nullopt;
        }

        return Placement{glyph_lines.toLines(), info_box};
    }
    case PlacementDirection::ABOVE: {
        info_box.x = anchor.x;

        glyph_lines.wrap(layout_width);
        info_box.width = glyph_lines.width();
        info_box.height = glyph_lines.height();

        if (info_box.height > MAX_HEIGHT)
        {
            glyph_lines.wrap(layout_width - scrollbar_space);
            info_box.width = glyph_lines.width() + scrollbar_space;
            info_box.height = MAX_HEIGHT;
        }

        info_box.y = anchor.y - info_box.height - SPACING_MEDIUM * 2.0f;

        if (info_box.y < 0)
        {
            return std::nullopt;
        }

        switch(alignment) {
            case CrossAxisAlignment::START:
                info_box.x = anchor.x;
                break;
            case CrossAxisAlignment::CENTER:
                info_box.x = anchor.x + (anchor.width - info_box.width) / 2.0f;
                break;
            case CrossAxisAlignment::END:
                info_box.x = anchor.x + anchor.width - info_box.width;
                break;
        }

        if (info_box.x + info_box.width + SPACING_MEDIUM * 2.0f > m_kakoune_content_view->width())
        {
            info_box.x = std::max(0.0f, m_kakoune_content_view->width() - info_box.width - SPACING_MEDIUM * 2.0f);
        }

        if (info_box.intersects(menu_rectangle)) {
            return std::nullopt;
        }

        if (has_cursor && info_box.intersects(cursor_rect)) {
            return std::nullopt;
        }

        return Placement{glyph_lines.toLines(), info_box};
    }
    case PlacementDirection::BELOW: {
        info_box.x = anchor.x;
        info_box.y = anchor.y + anchor.height;

        glyph_lines.wrap(layout_width);
        info_box.width = glyph_lines.width();
        info_box.height = glyph_lines.height();

        if (info_box.height > MAX_HEIGHT)
        {
            glyph_lines.wrap(layout_width - scrollbar_space);
            info_box.width = glyph_lines.width() + scrollbar_space;
            info_box.height = MAX_HEIGHT;
        }

        if (info_box.y + info_box.height + SPACING_MEDIUM * 2.0f > layout_height)
        {
            return std::nullopt;
        }

        switch(alignment) {
            case CrossAxisAlignment::START:
                info_box.x = anchor.x;
                break;
            case CrossAxisAlignment::CENTER:
                info_box.x = anchor.x + (anchor.width - info_box.width) / 2.0f;
                break;
            case CrossAxisAlignment::END:
                info_box.x = anchor.x + anchor.width - info_box.width;
                break;
        }

        if (info_box.x + info_box.width + SPACING_MEDIUM * 2.0f > m_kakoune_content_view->width())
        {
            info_box.x = std::max(0.0f, m_kakoune_content_view->width() - info_box.width - SPACING_MEDIUM * 2.0f);
        }

        if (info_box.intersects(menu_rectangle)) {
            return std::nullopt;
        }

        if (has_cursor && info_box.intersects(cursor_rect)) {
            return std::nullopt;
        }

        return Placement{glyph_lines.toLines(), info_box};
    }
    case PlacementDirection::CENTER: {
        if (info_box.height > MAX_HEIGHT)
        {
            float wrap_width = info_box.width - scrollbar_space;
            glyph_lines.wrap(wrap_width);
            info_box.width = glyph_lines.width() + scrollbar_space;
            info_box.height = MAX_HEIGHT;
        }

        info_box.x = anchor.x + (anchor.width - info_box.width) / 2.0f;
        info_box.y = anchor.y + (anchor.height - info_box.height) / 2.0f;

        if (info_box.intersects(menu_rectangle)) {
            return std::nullopt;
        }

        if (has_cursor && info_box.intersects(cursor_rect)) {
            return std::nullopt;
        }

        return Placement{glyph_lines.toLines(), info_box};
    }
    case PlacementDirection::FULL: {
        info_box.x = 0;
        info_box.y = 0;
        info_box.width = layout_width;
        info_box.height = layout_height;

        return Placement{content, info_box};
    }
    }
    return std::nullopt;
}

void InfoBoxView::render(const KakouneClient *kakoune_client, domain::FontManager* font_manager, const UIOptions& ui_options, float width, float height)
{
    domain::Rectangle anchor;
    PlacementDirection direction = PlacementDirection::BELOW;
    CrossAxisAlignment alignment = CrossAxisAlignment::START;
    std::vector<PlacementDirection> fallback_directions;

    switch (kakoune_client->state.info_box->style)
    {
    case domain::InfoStyle::PROMPT:
        if (kakoune_client->state.menu.has_value())
        {
            anchor = {m_menu_controller->x(), m_menu_controller->y(), m_menu_controller->width(),
                      m_menu_controller->height()};
            direction = PlacementDirection::BELOW;
            alignment = CrossAxisAlignment::START;
            fallback_directions = {PlacementDirection::ABOVE};
        }
        else
        {
            anchor = {0, m_kakoune_content_view->height(), m_kakoune_content_view->width(), 0};
            direction = PlacementDirection::ABOVE;
            alignment = CrossAxisAlignment::END;
            fallback_directions = {PlacementDirection::FULL};
        }
        break;

    case domain::InfoStyle::INLINE: {
        auto pos = m_kakoune_content_view->coordToPixels(ui_options.font, kakoune_client->state.info_box->anchor);
        anchor = {pos.first, pos.second, m_kakoune_content_view->getCellWidth(ui_options.font),
                  m_kakoune_content_view->getCellHeight(ui_options.font)};
        direction = PlacementDirection::BELOW;
        alignment = CrossAxisAlignment::START;
        fallback_directions = {PlacementDirection::ABOVE, PlacementDirection::RIGHT_WRAPPED,
                               PlacementDirection::LEFT_WRAPPED};
    }
    break;

    case domain::InfoStyle::INLINE_ABOVE: {
        auto pos = m_kakoune_content_view->coordToPixels(ui_options.font, kakoune_client->state.info_box->anchor);
        anchor = {pos.first, pos.second, m_kakoune_content_view->getCellWidth(ui_options.font),
                  m_kakoune_content_view->getCellHeight(ui_options.font)};
        direction = PlacementDirection::ABOVE;
        alignment = CrossAxisAlignment::START;
        fallback_directions = {PlacementDirection::BELOW, PlacementDirection::RIGHT_WRAPPED,
                               PlacementDirection::LEFT_WRAPPED};
    }
    break;

    case domain::InfoStyle::INLINE_BELOW: {
        auto pos = m_kakoune_content_view->coordToPixels(ui_options.font, kakoune_client->state.info_box->anchor);
        anchor = {pos.first, pos.second, 0, m_kakoune_content_view->getCellHeight(ui_options.font)};
        direction = PlacementDirection::BELOW;
        alignment = CrossAxisAlignment::START;
        fallback_directions = {PlacementDirection::ABOVE, PlacementDirection::RIGHT_WRAPPED,
                               PlacementDirection::LEFT_WRAPPED};
    }
    break;

    case domain::InfoStyle::MENUDOC:
        anchor = {m_menu_controller->x(), m_menu_controller->y(), m_menu_controller->width(),
                  m_menu_controller->height()};
        direction = PlacementDirection::RIGHT;
        alignment = CrossAxisAlignment::START;
        fallback_directions = {PlacementDirection::LEFT, PlacementDirection::RIGHT_WRAPPED,
                               PlacementDirection::LEFT_WRAPPED};
        break;

    case domain::InfoStyle::MODAL:
        anchor = {0, 0, width, height};
        direction = PlacementDirection::CENTER;
        alignment = CrossAxisAlignment::CENTER;
        fallback_directions = {PlacementDirection::FULL};
        break;
    }

    fallback_directions.insert(fallback_directions.begin(), direction);

    domain::Rectangle menu_rectangle = {
        m_menu_controller->x(),
        m_menu_controller->y(),
        m_menu_controller->width(),
        m_menu_controller->height(),
    };

    Placement placement;
    for (const auto& dir : fallback_directions)
    {
        auto current_placement = tryPlaceInfoBox(dir, alignment, kakoune_client->state.info_box->content, anchor, width, height, ui_options.font, font_manager, menu_rectangle, kakoune_client->state.cursor_position);
        if (current_placement.has_value()) {
            placement = current_placement.value();
            break;
        }
    }

    float infobox_height = placement.bounds.height + SPACING_MEDIUM * 2.0f + BORDER_THICKNESS * 2.0f;

    float title_height = ui_options.font->getLineHeight() + BORDER_THICKNESS + SPACING_SMALL + SPACING_MEDIUM;
    if (kakoune_client->state.info_box->title.size() > 0) {
        infobox_height += title_height;
    }

    LayoutManager layout(placement.bounds.x, placement.bounds.y, placement.bounds.width + SPACING_MEDIUM * 2.0f + BORDER_THICKNESS * 2.0f,
                         infobox_height);

    m_x = layout.current().x;
    m_y = layout.current().y;
    m_width = layout.current().width;
    m_height = layout.current().height;

    m_renderer->renderRect(domain::RGBAColor{0.5, 0.5, 0.5, 1},
                           layout.current().x, layout.current().y, layout.current().width, layout.current().height);

    layout.pad(BORDER_THICKNESS);

    m_renderer->renderRect(kakoune_client->state.info_box->default_face.getBg(kakoune_client->state.default_face),
                           layout.current().x, layout.current().y, layout.current().width, layout.current().height);

    layout.pad(SPACING_MEDIUM);

    if (kakoune_client->state.info_box->title.size() > 0) {
        m_renderer->renderLine(ui_options.font, font_manager, kakoune_client->state.info_box->title, kakoune_client->state.info_box->default_face, layout.current().x, layout.current().y);

        layout.gapY(ui_options.font->getLineHeight());

        layout.gapY(SPACING_SMALL);

        m_renderer->renderRect(domain::RGBAColor{0.5, 0.5, 0.5, 1}, layout.current().x, layout.current().y, layout.current().width, 1);

        layout.gapY(BORDER_THICKNESS);

        layout.gapY(SPACING_MEDIUM);

    }

    float line_height = ui_options.font->getLineHeight();
    int total_lines = placement.content.size();
    int visible_lines = static_cast<int>(placement.bounds.height / line_height);
    bool needs_scroll = total_lines > visible_lines;

    int max_scroll = std::max(0, total_lines - visible_lines);
    m_scroll_offset = std::max(0.0f, std::min(static_cast<float>(max_scroll), m_scroll_offset));

    auto content_layout = layout.copy();
    if (needs_scroll)
    {
        content_layout.padRight(SPACING_MEDIUM + m_scroll_bar->width());
    }

    int start_line = static_cast<int>(m_scroll_offset);
    int end_line = std::min(start_line + visible_lines, total_lines);

    float y_pos = content_layout.current().y;
    for (int i = start_line; i < end_line; i++)
    {
        m_renderer->renderLine(ui_options.font, font_manager, placement.content.at(i),
                             kakoune_client->state.info_box->default_face,
                             content_layout.current().x, y_pos);
        y_pos += line_height;
    }

    if (needs_scroll)
    {
        int max_scroll = total_lines - visible_lines;
        m_scroll_bar->setValue(m_scroll_offset, max_scroll, visible_lines);
        m_scroll_bar->render(m_renderer,
                           kakoune_client->state.info_box->default_face.getFg(kakoune_client->state.default_face),
                           layout);
    }
}

void InfoBoxView::onMouseScroll(int scroll_amount)
{
    m_scroll_offset += scroll_amount;
}

float InfoBoxView::x() const {
    return m_x;
}

float InfoBoxView::y() const {
    return m_y;
}

float InfoBoxView::width() const {
    return m_width;
}

float InfoBoxView::height() const {
    return m_height;
}
