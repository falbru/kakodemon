#include "infobox.hpp"
#include "controller/menucontroller.hpp"
#include "core/utf8string.hpp"
#include "spdlog/spdlog.h"
#include "view/glyphsequence.hpp"
#include "view/layoutmanager.hpp"
#include "view/promptmenu.hpp"
#include "view/styling.hpp"
#include <atomic>

InfoBoxView::InfoBoxView()
{
    m_font = std::make_shared<opengl::Font>("/home/falk/.fonts/MonoLisa/ttf/MonoLisa-Regular.ttf", 14);
}

void InfoBoxView::init(std::shared_ptr<opengl::Renderer> renderer,
                       std::shared_ptr<MenuController> menu_controller,
                       std::shared_ptr<KakouneContentView> kakoune_content_view)
{
    m_renderer = renderer;
    m_menu_controller = menu_controller;
    m_kakoune_content_view = kakoune_content_view;
}

void InfoBoxView::render(const KakouneClient &kakoune_client, float width, float height)
{
    const kakoune::Line *longest_line =
        &*std::max_element(kakoune_client.info_box_content.begin(), kakoune_client.info_box_content.end(),
                           [](const auto &a, const auto &b) { return a.size() < b.size(); });

    float info_box_width = std::min(MAX_WIDTH + SPACING_MEDIUM * 2, m_font->width(longest_line->toUTF8String()));
    float info_box_height = m_font->getLineHeight() * kakoune_client.info_box_content.size() + SPACING_MEDIUM * 2;

    float info_box_x;
    float info_box_y;

    switch(kakoune_client.info_box_style) {
        case kakoune::InfoStyle::PROMPT:
            // info_box_x = m_menu_controller->x() + m_menu_controller->width() + SPACING_XL;
            // info_box_y = m_menu_controller->y();
            info_box_x = m_menu_controller->x();
            info_box_y = m_menu_controller->y() + m_menu_controller->height() + SPACING_XL;
            break;
        case kakoune::InfoStyle::INLINE_ABOVE:
        case kakoune::InfoStyle::INLINE_BELOW:
        case kakoune::InfoStyle::INLINE:
            {
                auto pos = m_kakoune_content_view->coordToPixels(kakoune_client.info_box_anchor);
                info_box_x = pos.first;
                info_box_y = pos.second;
            }
            break;
        case kakoune::InfoStyle::MENUDOC:
            info_box_x = m_menu_controller->x() + m_menu_controller->width();
            info_box_y = m_menu_controller->y();
            break;
        case kakoune::InfoStyle::MODAL:
            break;
    }

    LayoutManager layout(info_box_x, info_box_y, info_box_width, info_box_height);

    m_renderer->renderRect(
        kakoune_client.info_box_face.bg.toCoreColor(kakoune_client.window_default_face.bg, false), layout.current().x,
        layout.current().y, layout.current().width, layout.current().height);

    layout.pad(SPACING_MEDIUM);

    std::vector<kakoune::Line> lines;
    const float max_width = layout.current().width;

    for (const auto& input_line : kakoune_client.info_box_content) {
        std::optional<kakoune::Line> current_line = input_line;

        while (current_line.has_value()) {
            std::vector<kakoune::Atom> wrapped_line;
            float current_width = 0;
            auto atom_it = current_line->atoms.begin();

            // Add complete atoms that fit
            while (atom_it != current_line->atoms.end()) {
                float atom_width = m_font->width(atom_it->contents);
                if (current_width + atom_width > max_width) break;

                current_width += atom_width;
                wrapped_line.push_back(*atom_it);
                ++atom_it;
            }

            // Handle partial atom if needed
            if (atom_it != current_line->atoms.end()) {
                UTF8String fitted_part;
                int last_whitespace_pos = -1;

                // Find maximum characters that fit
                for (int i = 0; i < atom_it->contents.size(); ++i) {
                    UTF8String test_string = fitted_part;
                    test_string.addCodepoint(atom_it->contents.at(i));

                    if (atom_it->contents.at(i) == 0x20) {
                        last_whitespace_pos = i;
                    }else if (m_font->width(test_string) > max_width - current_width) break;

                    fitted_part = test_string;
                }

                if (last_whitespace_pos > 0) {
                    wrapped_line.push_back(kakoune::Atom{atom_it->face, fitted_part.substring(0, last_whitespace_pos)});
                }

                // Create remainder line
                std::vector<kakoune::Atom> remainder_atoms;
                if (last_whitespace_pos < atom_it->contents.size()) {
                    remainder_atoms.push_back(kakoune::Atom{atom_it->face, atom_it->contents.substring(last_whitespace_pos+1)});
                }
                remainder_atoms.insert(remainder_atoms.end(), ++atom_it, current_line->atoms.end());

                current_line = remainder_atoms.empty() ? std::nullopt : std::make_optional<kakoune::Line>(kakoune::Line{remainder_atoms});
            } else {
                current_line = std::nullopt;
            }

            lines.push_back(kakoune::Line{wrapped_line});
        }
    }

    m_renderer->renderLines(m_font, lines, kakoune_client.info_box_face, layout.current().x,
                            layout.current().y);
}
