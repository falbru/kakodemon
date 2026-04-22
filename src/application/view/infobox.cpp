#include "application/view/rendercontext.hpp"
#include "application/view/styling.hpp"
#include "domain/editor.hpp"
#include "domain/geometry.hpp"
#include "domain/glyphline.hpp"
#include "domain/glyphlines.hpp"
#include "domain/glyphlinesbuilder.hpp"
#include "domain/infobox.hpp"
#include "infobox.hpp"
#include <algorithm>
#include <cstddef>
#include <optional>
#include <queue>
#include <spdlog/spdlog.h>
#include <variant>

InfoBoxView::InfoBoxView()
{
    m_scroll_bar = std::make_unique<ScrollBar>();
}

void InfoBoxView::init(domain::Renderer *renderer, MultiStyledMenuView *multi_styled_menu,
                       KakouneContentView *kakoune_content_view, StatusBarView *status_bar_view)
{
    m_renderer = renderer;
    m_multi_styled_menu = multi_styled_menu;
    m_kakoune_content_view = kakoune_content_view;
    m_status_bar_view = status_bar_view;
}

PlacementConfig InfoBoxView::placementConfigByInfoBoxStyle(const RenderContext &render_context, InfoBoxViewState &state,
                                                           const domain::InfoBox &info_box,
                                                           const domain::CursorPosition &cursor_position,
                                                           const domain::Rectangle &content_bounds, int info_box_width,
                                                           int info_box_height)
{
    domain::Rectangle anchor;
    std::vector<PlacementDirection> preferred_directions;

    switch (info_box.style)
    {
    case domain::InfoStyle::PROMPT:
        if (m_multi_styled_menu->isVisible())
        {
            return {
                AnchorPlacement{{static_cast<int>(m_multi_styled_menu->x()), static_cast<int>(m_multi_styled_menu->y()) - SPACING_MEDIUM,
                                 static_cast<int>(m_multi_styled_menu->width()),
                                 static_cast<int>(m_multi_styled_menu->height()) + SPACING_MEDIUM * 2},
                                {PlacementDirection::BELOW}},
                true};
        }
        else
        {
            return {
                FixedPlacement{{static_cast<int>(render_context.screen_width - info_box_width),
                                static_cast<int>(render_context.screen_height - m_status_bar_view->height(render_context.ui_options.font_statusbar) - info_box_height),
                                info_box_width, info_box_height}},
                true};
        }

    case domain::InfoStyle::INLINE: {
        auto pos = m_kakoune_content_view->coordToPixels(render_context.ui_options.font_content, info_box.anchor,
                                                         content_bounds.left(), content_bounds.top());
        return {AnchorPlacement{
                    {static_cast<int>(pos.first), static_cast<int>(pos.second),
                     static_cast<int>(m_kakoune_content_view->getCellWidth(render_context.ui_options.font_content)),
                     static_cast<int>(m_kakoune_content_view->getCellHeight(render_context.ui_options.font_content))},
                    {PlacementDirection::BELOW, PlacementDirection::ABOVE, PlacementDirection::RIGHT,
                     PlacementDirection::LEFT}},
                false};
    }

    case domain::InfoStyle::INLINE_ABOVE: {
        auto pos = m_kakoune_content_view->coordToPixels(render_context.ui_options.font_content, info_box.anchor,
                                                         content_bounds.left(), content_bounds.top());

        return {AnchorPlacement{
                    {static_cast<int>(pos.first), static_cast<int>(pos.second),
                     static_cast<int>(m_kakoune_content_view->getCellWidth(render_context.ui_options.font_content)),
                     static_cast<int>(m_kakoune_content_view->getCellHeight(render_context.ui_options.font_content))},
                    {PlacementDirection::ABOVE, PlacementDirection::BELOW, PlacementDirection::RIGHT,
                     PlacementDirection::LEFT}},
                false};
    }

    case domain::InfoStyle::INLINE_BELOW: {
        auto pos = m_kakoune_content_view->coordToPixels(render_context.ui_options.font_content, info_box.anchor,
                                                         content_bounds.left(), content_bounds.top());
        return {AnchorPlacement{
                    {static_cast<int>(pos.first), static_cast<int>(pos.second), 0,
                     static_cast<int>(m_kakoune_content_view->getCellHeight(render_context.ui_options.font_content))},
                    {PlacementDirection::BELOW, PlacementDirection::ABOVE, PlacementDirection::RIGHT,
                     PlacementDirection::LEFT}},
                false};
    }

    case domain::InfoStyle::MENUDOC:
        return {AnchorPlacement{{static_cast<int>(m_multi_styled_menu->x()), static_cast<int>(m_multi_styled_menu->y()),
                                 static_cast<int>(m_multi_styled_menu->width()),
                                 static_cast<int>(m_multi_styled_menu->height())},
                                {PlacementDirection::RIGHT, PlacementDirection::LEFT, PlacementDirection::ABOVE,
                                 PlacementDirection::BELOW}},
                false};

    case domain::InfoStyle::MODAL:
        return {FixedPlacement{domain::Rectangle{(int)((render_context.screen_width - info_box_width) / 2.0f),
                                                 (int)((render_context.screen_height - info_box_height) / 2.0f),
                                                 info_box_width, info_box_height}},
                true};
    }
}

std::optional<domain::Rectangle> InfoBoxView::placeInfoBox(const RenderContext &render_context, InfoBoxViewState &state,
                                                           const domain::InfoBox &info_box,
                                                           const domain::CursorPosition &cursor_position,
                                                           const domain::Rectangle &content_bounds, int info_box_width,
                                                           int info_box_height)
{
    auto placement_config = placementConfigByInfoBoxStyle(render_context, state, info_box, cursor_position,
                                                          content_bounds, info_box_width, info_box_height);

    if (std::holds_alternative<FixedPlacement>(placement_config.placement))
    {
        auto &fixed = std::get<FixedPlacement>(placement_config.placement);
        return fixed.placement;
    }

    auto &anchor_placement = std::get<AnchorPlacement>(placement_config.placement);
    auto anchor = anchor_placement.anchor;
    auto preferred_directions = anchor_placement.preferred_directions;

    domain::Rectangle menu_rectangle = {
        static_cast<int>(m_multi_styled_menu->x()),
        static_cast<int>(m_multi_styled_menu->y()),
        static_cast<int>(m_multi_styled_menu->width()),
        static_cast<int>(m_multi_styled_menu->height()),
    };

    std::optional<domain::Rectangle> cursor_line = std::nullopt;
    if (std::holds_alternative<domain::BufferContentPosition>(cursor_position))
    {
        std::pair<float, float> pos = m_kakoune_content_view->coordToPixels(
            render_context.ui_options.font_content, std::get<domain::BufferContentPosition>(cursor_position).coord,
            content_bounds.left(), content_bounds.top());

        cursor_line = domain::Rectangle(0, pos.second, 1000,
                                        m_kakoune_content_view->getCellHeight(render_context.ui_options.font_content));
    }

    return placeWithoutOverlap(content_bounds, domain::IVec2{info_box_width, info_box_height}, anchor, preferred_directions,
                           {
                               m_multi_styled_menu->isVisible() ? menu_rectangle : domain::Rectangle(),
                               cursor_line.value_or(domain::Rectangle()),
                           });
}

void InfoBoxView::render(const RenderContext &render_context, InfoBoxViewState &state, const domain::InfoBox &info_box,
                         const domain::CursorPosition &cursor_position, const domain::Rectangle &content_bounds)
{
    domain::Font *font = render_context.ui_options.font_infobox;

    auto glyph_lines = domain::GlyphLinesBuilder::build(info_box.content, render_context.ui_options.font_infobox);
    glyph_lines.wrap(MAX_WIDTH, domain::WrapMode::WORD);

    int info_box_width = static_cast<int>(std::max(glyph_lines.width(), domain::GlyphLinesBuilder::build(info_box.title, render_context.ui_options.font_infobox).width())) + SPACING_MEDIUM * 2 + BORDER_THICKNESS * 2;

    int info_box_height = (int)(glyph_lines.height() + SPACING_MEDIUM * 2 + BORDER_THICKNESS * 2);
    if (info_box.title.size() > 0) {
        info_box_height += font->getLineHeight() + BORDER_THICKNESS + SPACING_SMALL + SPACING_MEDIUM;
    }
    info_box_height = std::min(info_box_height, MAX_HEIGHT);

    auto placement = placeInfoBox(render_context, state, info_box, cursor_position, content_bounds, info_box_width, info_box_height);
    if (!placement)
    {
        return;
    }

    LayoutManager layout(placement->left(), placement->top(), placement->width(), placement->height());

    m_x = layout.current().x;
    m_y = layout.current().y;
    m_width = layout.current().width;
    m_height = layout.current().height;

    m_renderer->renderRect(getRGBAColor(render_context.ui_options.color_border, default_border_color),
                           layout.current().x, layout.current().y, layout.current().width, layout.current().height);

    layout.pad(BORDER_THICKNESS);

    m_renderer->renderRect(
        info_box.default_face.getBg(render_context.default_face, render_context.ui_options.color_overrides),
        layout.current().x, layout.current().y, layout.current().width, layout.current().height);

    layout.pad(SPACING_MEDIUM);

    if (info_box.title.size() > 0)
    {
        m_renderer->renderLine(render_context.textConfig(font), info_box.title, info_box.default_face,
                               layout.current().x, layout.current().y);

        layout.gapY(font->getLineHeight());

        layout.gapY(SPACING_SMALL);

        m_renderer->renderRect(getRGBAColor(render_context.ui_options.color_border, default_border_color),
                               layout.current().x, layout.current().y, layout.current().width, 1);

        layout.gapY(BORDER_THICKNESS);

        layout.gapY(SPACING_MEDIUM);
    }

    float line_height = font->getLineHeight();
    auto lines = glyph_lines.toLines();
    int total_lines = lines.size();
    int visible_lines = static_cast<int>(placement->height() / line_height);
    bool needs_scroll = total_lines > visible_lines;

    int max_scroll = std::max(0, total_lines - visible_lines);
    state.scroll_offset = std::max(0.0f, std::min(static_cast<float>(max_scroll), state.scroll_offset));

    auto content_layout = layout.copy();
    if (needs_scroll)
    {
        content_layout.padRight(SPACING_MEDIUM + m_scroll_bar->width());
    }

    int start_line = static_cast<int>(state.scroll_offset);
    int end_line = std::min(start_line + visible_lines, total_lines);

    float y_pos = content_layout.current().y;
    for (int i = start_line; i < end_line; i++)
    {
        m_renderer->renderLine(render_context.textConfig(font), lines.at(i), info_box.default_face,
                               content_layout.current().x, y_pos);
        y_pos += line_height;
    }

    if (needs_scroll)
    {
        int max_scroll = total_lines - visible_lines;
        m_scroll_bar->setValue(state.scroll_offset, max_scroll, visible_lines);
        m_scroll_bar->render(
            m_renderer,
            info_box.default_face.getFg(render_context.default_face, render_context.ui_options.color_overrides),
            layout);
    }
}

std::optional<domain::Rectangle> InfoBoxView::placeWithoutOverlap(
    const domain::Rectangle &bounds, const domain::IVec2 &fitted_rectangle_size, const domain::Rectangle &anchor,
    const std::vector<PlacementDirection> &preferred_directions, const std::vector<domain::Rectangle> &obstacles)
{
    std::vector<domain::Rectangle> obstacles_with_anchor;
    obstacles_with_anchor.reserve(obstacles.size() + 1);
    obstacles_with_anchor.insert(obstacles_with_anchor.begin(), obstacles.begin(), obstacles.end());
    obstacles_with_anchor.push_back(anchor);

    std::optional<domain::Rectangle> overall_best_placement = std::nullopt;

    int anchor_center_x = anchor.centerX();
    int anchor_center_y = anchor.centerY();
    auto compare_placements = [anchor_center_x, anchor_center_y](const domain::Rectangle &a,
                                                                 const domain::Rectangle &b) {
        auto area_a = a.width() * a.height();
        auto area_b = b.width() * b.height();
        if (area_a != area_b)
        {
            return area_a < area_b;
        }
        else
        {
            return a.distanceSquared(anchor_center_x, anchor_center_y) >
                   b.distanceSquared(anchor_center_x, anchor_center_y);
        }
    };

    auto it = preferred_directions.begin();
    while (it != preferred_directions.end())
    {
        PlacementDirection direction = *it++;

        domain::IVec2 start_position = domain::IVec2{anchor.left(), anchor.top()};

        switch (direction)
        {
        case PlacementDirection::ABOVE:
            start_position.y -= fitted_rectangle_size.y;
            break;
        case PlacementDirection::BELOW:
            start_position.y = anchor.bottom();
            break;
        case PlacementDirection::LEFT:
            start_position.x -= fitted_rectangle_size.x;
            break;
        case PlacementDirection::RIGHT:
            start_position.x = anchor.right();
            break;
        }

        std::vector<domain::Rectangle> valid_placements =
            findNonOverlappingPlacements(bounds, fitted_rectangle_size, start_position, obstacles_with_anchor);

        if (valid_placements.size() == 0)
        {
            continue;
        }

        auto best_placement = std::max_element(valid_placements.begin(), valid_placements.end(), compare_placements);

        if (best_placement == valid_placements.end())
        {
            continue;
        }

        if (!overall_best_placement.has_value() || compare_placements(overall_best_placement.value(), *best_placement))
        {
            overall_best_placement = *best_placement;
        }
    }

    return overall_best_placement;
}

const int MIN_PLACEMENT_WIDTH = 50;
const int MIN_PLACEMENT_HEIGHT = 50;

std::vector<domain::Rectangle> InfoBoxView::findNonOverlappingPlacements(const domain::Rectangle &bounds,
                                                                const domain::IVec2 &fitted_rectangle_size,
                                                                const domain::IVec2 &start_position,
                                                                const std::vector<domain::Rectangle> &obstacles)
{
    std::vector<domain::Rectangle> valid_placements;
    std::queue<domain::Rectangle> placements;
    std::vector<bool> visited(obstacles.size(), false);

    domain::Rectangle first_placement{start_position.x, start_position.y, fitted_rectangle_size.x,
                                      fitted_rectangle_size.y};
    if (first_placement.right() > bounds.right())
    {
        first_placement.setX(bounds.right() - fitted_rectangle_size.x);
    }
    if (first_placement.left() < bounds.left())
    {
        first_placement.setX(bounds.left());
    }
    if (first_placement.bottom() > bounds.bottom())
    {
        first_placement.setY(bounds.bottom() - fitted_rectangle_size.y);
    }
    if (first_placement.top() < bounds.top())
    {
        first_placement.setY(bounds.top());
    }
    placements.push(first_placement);

    while (!placements.empty())
    {
        auto placement = placements.front();
        placements.pop();

        bool any_intersection = false;
        for (std::size_t i = 0; i < obstacles.size(); i++)
        {
            auto &obstacle = obstacles[i];

            auto intersection = placement.intersection(obstacle);
            if (intersection.has_value())
            {
                any_intersection = true;

                if (!visited[i])
                {
                    visited[i] = true;

                    std::vector<domain::Rectangle> push_rectangles = {
                        domain::Rectangle(obstacle.right(), placement.top(), placement.width(), placement.height()),
                        domain::Rectangle(obstacle.left() - placement.width(), placement.top(), placement.width(),
                                          placement.height()),
                        domain::Rectangle(placement.left(), obstacle.top() - placement.height(), placement.width(),
                                          placement.height()),
                        domain::Rectangle(placement.left(), obstacle.bottom(), placement.width(), placement.height()),
                    };

                    for (domain::Rectangle &rect : push_rectangles)
                    {
                        if (rect.left() < bounds.left())
                        {
                            rect.setWidth(rect.right() - bounds.left());
                            rect.setX(bounds.left());
                        }
                        if (rect.right() > bounds.right())
                        {
                            rect.setWidth(bounds.right() - rect.left());
                        }
                        if (rect.top() < bounds.top())
                        {
                            rect.setHeight(rect.bottom() - bounds.top());
                            rect.setY(bounds.top());
                        }
                        if (rect.bottom() > bounds.bottom())
                        {
                            rect.setHeight(bounds.bottom() - rect.top());
                        }

                        if (rect.width() > MIN_PLACEMENT_WIDTH && rect.height() > MIN_PLACEMENT_HEIGHT)
                        {
                            placements.push(rect);
                        }
                    }
                }
            }
        }

        if (!any_intersection)
        {
            valid_placements.push_back(placement);
        }
    }

    return std::move(valid_placements);
}

void InfoBoxView::handleMouseScroll(InfoBoxViewState &state, int scroll_amount)
{
    state.scroll_offset += scroll_amount;
}

float InfoBoxView::x() const
{
    return m_x;
}

float InfoBoxView::y() const
{
    return m_y;
}

float InfoBoxView::width() const
{
    return m_width;
}

float InfoBoxView::height() const
{
    return m_height;
}
