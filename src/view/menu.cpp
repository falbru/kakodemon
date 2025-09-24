#include "view/layoutmanager.hpp"
#include "view/menu.hpp"
#include <spdlog/spdlog.h>

MenuView::MenuView()
    : m_font("/home/falk/.fonts/MonoLisa/ttf/MonoLisa-Regular.ttf", 15),
      m_scroll_offset(0) {}

void MenuView::init(std::shared_ptr<opengl::Renderer> renderer) {
  m_renderer = renderer;
}

void MenuView::render(const KakouneClient &kakoune_client, float width,
                      float height) {
  if (kakoune_client.status_line.atoms.size() < 1)
    return;
  static bool opened_before = false;
  if (!kakoune_client.menu_visible && !opened_before)
      return;
  opened_before = true;

  float menu_width = 600.0f;
  float menu_x = (width - menu_width) / 2;
  float menu_y = 60.0f;
  float padding = 5.0f;
  float line_height = 21.0f;

  float input_border = 1.0f;
  float input_padding = 5.0f;
  float gap = 5.0f;

  float items_length = max_items < kakoune_client.menu_items.size() ? max_items : kakoune_client.menu_items.size();
  float menu_height = padding * 2 + line_height + input_border * 2 + input_padding * 2;
  if (kakoune_client.menu_visible) {
     menu_height += line_height * items_length + gap;
  }

  LayoutManager layout(menu_x, menu_y, menu_width, menu_height);

  m_renderer->renderRectWithShadow(
      kakoune_client.menu_face.bg.toCoreColor(
          kakoune_client.window_default_face.bg, false),
      layout.current().x, layout.current().y, layout.current().width,
      layout.current().height, 15.0f);

  layout.pad(padding);

  // INPUT
  auto input_layout =
      layout.sliceTop(line_height + input_padding * 2 + input_border * 2);

  m_renderer->renderRect(kakoune_client.menu_selected_face.bg.toCoreColor(
                             kakoune_client.window_default_face.bg, false),
                         input_layout.current().x, input_layout.current().y,
                         input_layout.current().width,
                         input_layout.current().height);

  input_layout.border(input_border);

  m_renderer->renderRect(kakoune_client.status_default_face.bg.toCoreColor(
                             kakoune_client.window_default_face.bg, false),
                         input_layout.current().x, input_layout.current().y,
                         input_layout.current().width,
                         input_layout.current().height);

  input_layout.pad(input_padding);

  m_renderer->renderLine(m_font, kakoune_client.status_line,
                         kakoune_client.status_default_face,
                         input_layout.current().x, input_layout.current().y);

  // ITEMS
  if (!kakoune_client.menu_visible) return;

  layout.gapY(gap);

  auto items_layout = layout.copy();

  if (kakoune_client.menu_selected_index < m_scroll_offset) {
    m_scroll_offset = std::max(0, kakoune_client.menu_selected_index);
  } else if (kakoune_client.menu_selected_index >=
             m_scroll_offset + max_items) {
    m_scroll_offset = std::max(0, kakoune_client.menu_selected_index - max_items + 1);
  }

  for (int i = m_scroll_offset; i < m_scroll_offset + max_items && i < kakoune_client.menu_items.size(); i++) {
    auto item = kakoune_client.menu_items.at(i);

    if (i == kakoune_client.menu_selected_index) {
      m_renderer->renderRect(kakoune_client.menu_selected_face.bg.toCoreColor(
                                 kakoune_client.window_default_face.bg, false),
                             menu_x, items_layout.current().y, menu_width,
                             line_height);
    }
    m_renderer->renderLine(m_font, item,
                           i == kakoune_client.menu_selected_index
                               ? kakoune_client.menu_selected_face
                               : kakoune_client.menu_face,
                           items_layout.current().x, items_layout.current().y);
    items_layout.sliceTop(line_height);
  }

  if (kakoune_client.menu_items.size() > max_items) {
    float scroll_width = 5.0f;
    float scroll_height =
        ((float)max_items / kakoune_client.menu_items.size()) *
        (max_items * line_height);

    m_renderer->renderRect(
        kakoune_client.status_default_face.fg.toCoreColor(std::nullopt, true),
        layout.current().x + layout.current().width - scroll_width,
        layout.current().y + ((float)m_scroll_offset /
                              (kakoune_client.menu_items.size() - max_items)) *
                                 (max_items * line_height - scroll_height),
        scroll_width, scroll_height);
  }
}
