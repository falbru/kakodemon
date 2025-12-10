#include "controller/menucontroller.hpp"
#include "core/utf8string.hpp"
#include "infobox.hpp"
#include "kakoune/line.hpp"
#include "opengl/renderer.hpp"
#include "view/glyphsequence.hpp"
#include "view/layoutmanager.hpp"
#include "view/styling.hpp"
#include <optional>
#include <spdlog/spdlog.h>

InfoBoxView::InfoBoxView()
{
    m_font = std::make_shared<opengl::Font>("/usr/share/fonts/truetype/ubuntu/UbuntuMono-R.ttf", 14);
}

void InfoBoxView::init(std::shared_ptr<opengl::Renderer> renderer, std::shared_ptr<MenuController> menu_controller,
                       std::shared_ptr<KakouneContentView> kakoune_content_view)
{
    m_renderer = renderer;
    m_menu_controller = menu_controller;
    m_kakoune_content_view = kakoune_content_view;
}

std::pair<std::vector<kakoune::Line>, std::pair<float, float>> InfoBoxView::calculateWrappedContent(
    const std::vector<kakoune::Line> &input_lines, float max_width) const
{
    std::vector<kakoune::Line> lines;
    float actual_max_width = 0;

    for (const auto &input_line : input_lines)
    {
        std::optional<kakoune::Line> current_line = input_line;

        while (current_line.has_value())
        {
            std::vector<kakoune::Atom> wrapped_line;
            float current_width = 0;
            auto atom_it = current_line->atoms.begin();

            while (atom_it != current_line->atoms.end())
            {
                float atom_width = m_font->width(atom_it->contents);
                if (current_width + atom_width > max_width)
                    break;

                current_width += atom_width;
                wrapped_line.push_back(*atom_it);
                ++atom_it;
            }

            if (atom_it != current_line->atoms.end())
            {
                UTF8String fitted_part;
                int last_whitespace_pos = -1;

                for (int i = 0; i < atom_it->contents.size(); ++i)
                {
                    UTF8String test_string = fitted_part;
                    test_string.addCodepoint(atom_it->contents.at(i));

                    if (m_font->width(test_string) > max_width - current_width)
                        break;

                    fitted_part = test_string;

                    if (atom_it->contents.at(i) == 0x20)
                    {
                        last_whitespace_pos = i;
                    }
                }

                int break_pos;
                int remainder_start;

                if (last_whitespace_pos > 0 && fitted_part.size() > 0)
                {
                    break_pos = last_whitespace_pos;
                    remainder_start = last_whitespace_pos + 1;
                }
                else if (fitted_part.size() > 0)
                {
                    break_pos = fitted_part.size();
                    remainder_start = fitted_part.size();
                }
                else
                {
                    fitted_part.addCodepoint(atom_it->contents.at(0));
                    break_pos = 1;
                    remainder_start = 1;
                }

                if (break_pos > 0)
                {
                    wrapped_line.push_back(kakoune::Atom{atom_it->face, atom_it->contents.substring(0, break_pos)});
                }

                std::vector<kakoune::Atom> remainder_atoms;
                if (remainder_start < atom_it->contents.size())
                {
                    remainder_atoms.push_back(
                        kakoune::Atom{atom_it->face, atom_it->contents.substring(remainder_start)});
                }
                remainder_atoms.insert(remainder_atoms.end(), ++atom_it, current_line->atoms.end());

                current_line = remainder_atoms.empty()
                                   ? std::nullopt
                                   : std::make_optional<kakoune::Line>(kakoune::Line{remainder_atoms});
            }
            else
            {
                current_line = std::nullopt;
            }

            float line_width = 0;
            for (const auto &atom : wrapped_line)
            {
                line_width += m_font->width(atom.contents);
            }
            actual_max_width = std::max(actual_max_width, line_width);

            lines.push_back(kakoune::Line{wrapped_line});
        }
    }

    float height = m_font->getLineHeight() * lines.size();
    return {lines, {actual_max_width, height}};
}

std::optional<Placement> InfoBoxView::tryPlaceInfoBox(PlacementDirection direction, CrossAxisAlignment alignment,
                                                      const std::vector<kakoune::Line> &content,
                                                      const Rectangle &anchor, float layout_width, float layout_height)
{
    std::vector<kakoune::Line> new_content;
    Rectangle info_box;

    const kakoune::Line *longest_line =
        &*std::max_element(content.begin(), content.end(),
                           [](const auto &a, const auto &b) { return a.size() < b.size(); });

    info_box.width = GlyphSequence(m_font, longest_line->toUTF8String()).width();
    info_box.height = content.size() * m_font->getLineHeight();

    if (info_box.width > MAX_WIDTH - SPACING_MEDIUM * 2.0f)
    {
        auto [wrapped_content, dimensions] =
            calculateWrappedContent(content, MAX_WIDTH - SPACING_MEDIUM * 2.0f);

        new_content = wrapped_content;
        info_box.width = dimensions.first;
        info_box.height = dimensions.second;
    }


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

            auto [wrapped_content, dimensions] = calculateWrappedContent(content, available_width);

            new_content = wrapped_content;
            info_box.width = dimensions.first;
            info_box.height = dimensions.second;
        }
        else if (info_box.x + info_box.width + SPACING_MEDIUM * 2.0f > layout_width)
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

        return Placement{new_content, info_box};
    }
    case PlacementDirection::LEFT:
    case PlacementDirection::LEFT_WRAPPED: {
        if (direction == PlacementDirection::LEFT_WRAPPED)
        {
            float available_width = anchor.x - SPACING_MEDIUM * 2.0f;
            if (available_width < MIN_WIDTH)
                return std::nullopt;

            auto [wrapped_content, dimensions] = calculateWrappedContent(content, available_width);

            new_content = wrapped_content;
            info_box.width = dimensions.first;
            info_box.height = dimensions.second;
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

        return Placement{new_content, info_box};
    }
    case PlacementDirection::ABOVE: {
        info_box.x = anchor.x;
        info_box.y = anchor.y - info_box.height - SPACING_MEDIUM * 2.0f;

        auto [wrapped_content, dimensions] = calculateWrappedContent(content, layout_width);

        new_content = wrapped_content;
        info_box.width = dimensions.first;
        info_box.height = dimensions.second;

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

        return Placement{new_content, info_box};
    }
    case PlacementDirection::BELOW: {
        info_box.x = anchor.x;
        info_box.y = anchor.y + anchor.height;

        auto [wrapped_content, dimensions] = calculateWrappedContent(content, layout_width);

        new_content = wrapped_content;
        info_box.width = dimensions.first;
        info_box.height = dimensions.second;

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

        return Placement{new_content, info_box};
    }
    case PlacementDirection::CENTER: {
        info_box.x = anchor.x + (anchor.width - info_box.width) / 2.0f;
        info_box.y = anchor.y + (anchor.height - info_box.height) / 2.0f;

        return Placement{content, info_box};
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

void InfoBoxView::render(const KakouneClient &kakoune_client, float width, float height)
{
    if (kakoune_client.info_box_content.empty())
    {
        return;
    }

    Rectangle anchor;
    PlacementDirection direction = PlacementDirection::BELOW;
    CrossAxisAlignment alignment = CrossAxisAlignment::START;
    std::vector<PlacementDirection> fallback_directions;

    switch (kakoune_client.info_box_style)
    {
    case kakoune::InfoStyle::PROMPT:
        if (kakoune_client.menu_visible)
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

    case kakoune::InfoStyle::INLINE: {
        auto pos = m_kakoune_content_view->coordToPixels(kakoune_client.info_box_anchor);
        anchor = {pos.first, pos.second, m_kakoune_content_view->getCellWidth(),
                  m_kakoune_content_view->getCellHeight()};
        direction = PlacementDirection::BELOW;
        alignment = CrossAxisAlignment::START;
        fallback_directions = {PlacementDirection::ABOVE, PlacementDirection::RIGHT_WRAPPED,
                               PlacementDirection::LEFT_WRAPPED};
    }
    break;

    case kakoune::InfoStyle::INLINE_ABOVE: {
        auto pos = m_kakoune_content_view->coordToPixels(kakoune_client.info_box_anchor);
        anchor = {pos.first, pos.second, m_kakoune_content_view->getCellWidth(),
                  m_kakoune_content_view->getCellHeight()};
        direction = PlacementDirection::ABOVE;
        alignment = CrossAxisAlignment::START;
        fallback_directions = {PlacementDirection::BELOW, PlacementDirection::RIGHT_WRAPPED,
                               PlacementDirection::LEFT_WRAPPED};
    }
    break;

    case kakoune::InfoStyle::INLINE_BELOW: {
        auto pos = m_kakoune_content_view->coordToPixels(kakoune_client.info_box_anchor);
        anchor = {pos.first, pos.second, 0, m_kakoune_content_view->getCellHeight()};
        direction = PlacementDirection::BELOW;
        alignment = CrossAxisAlignment::START;
        fallback_directions = {PlacementDirection::ABOVE, PlacementDirection::RIGHT_WRAPPED,
                               PlacementDirection::LEFT_WRAPPED};
    }
    break;

    case kakoune::InfoStyle::MENUDOC:
        anchor = {m_menu_controller->x(), m_menu_controller->y(), m_menu_controller->width(),
                  m_menu_controller->height()};
        direction = PlacementDirection::RIGHT;
        alignment = CrossAxisAlignment::START;
        fallback_directions = {PlacementDirection::LEFT, PlacementDirection::RIGHT_WRAPPED,
                               PlacementDirection::LEFT_WRAPPED};
        break;

    case kakoune::InfoStyle::MODAL:
        anchor = {0, 0, width, height};
        direction = PlacementDirection::CENTER;
        alignment = CrossAxisAlignment::CENTER;
        fallback_directions = {PlacementDirection::FULL};
        break;
    }

    fallback_directions.insert(fallback_directions.begin(), direction);

    Placement placement;
    for (auto dir : fallback_directions)
    {
        auto current_placement = tryPlaceInfoBox(dir, alignment, kakoune_client.info_box_content, anchor, width, height);
        if (current_placement.has_value()) {
            placement = current_placement.value();
            break;
        }
    }

    LayoutManager layout(placement.bounds.x, placement.bounds.y, placement.bounds.width + SPACING_MEDIUM * 2.0f,
                         placement.bounds.height + SPACING_MEDIUM * 2.0f);

    m_renderer->renderRect(kakoune_client.info_box_face.bg.toCoreColor(kakoune_client.window_default_face.bg, false),
                           layout.current().x, layout.current().y, layout.current().width, layout.current().height);

    layout.pad(SPACING_MEDIUM);

    m_renderer->renderLines(m_font, placement.content, kakoune_client.info_box_face, layout.current().x, layout.current().y);
}
