#include "view/inlinemenu.hpp"
#include "core/utf8string.hpp"
#include "view/styling.hpp"

InlineMenuView::InlineMenuView() {
  m_font = std::make_shared<opengl::Font>("/home/falk/.fonts/MonoLisa/ttf/MonoLisa-Regular.ttf", 14);
  m_scroll_bar = std::make_unique<ScrollBar>();
}

void InlineMenuView::init(std::shared_ptr<opengl::Renderer> renderer, std::shared_ptr<KakouneContentView> kakoune_content_view) {
  m_renderer = renderer;
  m_kakoune_content_view = kakoune_content_view;
}

void InlineMenuView::render(const KakouneClient &kakoune_client, float width, float height) {
  auto anchor = kakoune_client.menu_anchor;

  int max_item_length = 0;
  for (const auto &item : kakoune_client.menu_items) {
    int item_length = item.size();

    if (item_length > max_item_length) {
      max_item_length = item_length;
    }
  }
  max_item_length = std::min(MAX_ITEM_LENGTH, max_item_length);

  auto menu_position = m_kakoune_content_view->coordToPixels(kakoune_client.menu_anchor);
  float menu_x = menu_position.first;

  float menu_width = 2 * SPACING_MEDIUM + max_item_length * m_kakoune_content_view->getCellWidth() + SPACING_MEDIUM + m_scroll_bar->width();
  float items_size = std::min(MAX_VISIBLE_ITEMS, (int)kakoune_client.menu_items.size());
  float menu_height = m_font->getLineHeight() * items_size;

  float menu_y = menu_position.second + SPACING_SMALL + m_font->getLineHeight();
  if (menu_y + menu_height > height) {
      menu_y = menu_position.second - SPACING_SMALL - menu_height;
  }

  LayoutManager layout(menu_x, menu_y, menu_width, menu_height);

  m_renderer->renderRect(kakoune_client.menu_face.bg.toCoreColor(kakoune_client.window_default_face.bg, false),
                         layout.current().x, layout.current().y, layout.current().width, layout.current().height);

  layout.pad(0, SPACING_MEDIUM);

  renderScrolledContent(kakoune_client, layout, MAX_VISIBLE_ITEMS);
}

void InlineMenuView::renderScrolledContent(const KakouneClient &kakoune_client,
                                     LayoutManager &layout, int max_items) {
  auto items_layout = layout.copy();
  float line_height = m_font->getLineHeight();

  int selected_index = kakoune_client.menu_selected_index;
  if (selected_index >= kakoune_client.menu_items.size())
    selected_index = -1;
  if (selected_index < m_scroll_offset) {
    m_scroll_offset = std::max(0, selected_index);
  } else if (selected_index >= m_scroll_offset + max_items) {
    m_scroll_offset = std::max(0, selected_index - max_items + 1);
  }

  for (int i = m_scroll_offset; i < m_scroll_offset + max_items && i < kakoune_client.menu_items.size(); i++) {
    auto item = kakoune_client.menu_items.at(i);

    UTF8String trimmed = item.atoms[0].contents;
    trimmed.trim(TrimDirection::Right);
    if (trimmed.size() > MAX_ITEM_LENGTH) {
        item.atoms[0].contents = trimmed.substring(0, MAX_ITEM_LENGTH - 1);
        item.atoms[0].contents.trim(TrimDirection::Right);
        item.atoms[0].contents.addCodepoint(0x2026); // …
    } else {
        item.atoms[0].contents = item.atoms[0].contents.substring(0, MAX_ITEM_LENGTH);
    }

    if (i == selected_index) {
      m_renderer->renderRect(kakoune_client.menu_selected_face.bg.toCoreColor(
                                 kakoune_client.window_default_face.bg, false),
                             items_layout.current().x - SPACING_MEDIUM, items_layout.current().y, items_layout.current().width + SPACING_MEDIUM * 2,
                             line_height);

      m_renderer->renderLine(*m_font, item, kakoune_client.menu_selected_face,
                             items_layout.current().x, items_layout.current().y);
    } else {
      m_renderer->renderLine(*m_font, item, kakoune_client.menu_face,
                             items_layout.current().x, items_layout.current().y);
    }
    items_layout.sliceTop(line_height);
  }

  if (kakoune_client.menu_items.size() > max_items) {
    m_scroll_bar->setValue(m_scroll_offset,
                           kakoune_client.menu_items.size() - max_items,
                           max_items);
    m_scroll_bar->render(
        m_renderer,
        kakoune_client.status_default_face.fg.toCoreColor(std::nullopt, true),
        layout);
  }
}
