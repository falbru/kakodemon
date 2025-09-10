#include "view/menu.hpp"

MenuView::MenuView() : m_font("/home/falk/.fonts/MonoLisa/ttf/MonoLisa-Regular.ttf", 15) {

}

void MenuView::init(std::shared_ptr<opengl::Renderer> renderer) {
    m_renderer = renderer;
}

void MenuView::render(const KakouneClient& kakoune_client,
                      float width,
                      float height)
{
    if (!kakoune_client.menu_visible || kakoune_client.status_line.atoms.size() < 1) return;

    float menu_width = 600.0f;
    float menu_x = (width-menu_width)/2;
    float menu_y = 60.0f;
    float padding = 5.0f;
    float line_height = 21.0f;

    float x_it = menu_x;
    float y_it = menu_y;
    float width_it = menu_width;

    m_renderer->renderRectWithShadow(kakoune_client.menu_face.bg.toCoreColor(kakoune_client.window_default_face.bg, false), x_it, y_it, width_it, height / 2, 15.0f);

    x_it += padding;
    y_it += padding;
    width_it -= padding * 2;
    float input_border = 1.0f;
    float input_padding = 5.0f;

    m_renderer->renderRect(kakoune_client.menu_selected_face.bg.toCoreColor(kakoune_client.window_default_face.bg, false), x_it, y_it, width_it, line_height + 2*input_border + input_padding*2);
    x_it += input_border;
    y_it += input_border;
    width_it -= input_border*2;

    m_renderer->renderRect(kakoune_client.status_default_face.bg.toCoreColor(kakoune_client.window_default_face.bg, false), x_it, y_it, width_it, line_height + input_padding*2);

    x_it += input_padding;
    y_it += input_padding;
    width_it -= input_padding * 2;

    m_renderer->renderRect(kakoune_client.status_default_face.bg.toCoreColor(std::nullopt, false), x_it, y_it, width_it, line_height);
    m_renderer->renderLine(m_font, kakoune_client.status_line, kakoune_client.status_default_face, x_it, y_it);

    float gap = 5.0f;
    x_it = menu_x + padding;
    width_it = menu_width - padding * 2.0f;
    y_it += line_height + input_padding + input_border + gap;

    if (!kakoune_client.menu_visible) return;

    int i = 0;
    for (auto item : kakoune_client.menu_items) {
        if (i == kakoune_client.menu_selected_index) {
            m_renderer->renderRect(kakoune_client.menu_selected_face.bg.toCoreColor(kakoune_client.window_default_face.bg, false), menu_x, y_it, menu_width, line_height);
        }
        m_renderer->renderLine(m_font, item, i == kakoune_client.menu_selected_index ? kakoune_client.menu_selected_face : kakoune_client.menu_face, x_it, y_it);
        y_it += line_height;
        if (y_it - menu_y + line_height > height / 2) {
            break;
        }
        i++;
    }
}
