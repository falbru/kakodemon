#include "kakoune/menustyle.hpp"
#include "model/kakouneclient.hpp"
#include "opengl/font.hpp"
#include "view/kakounecontentview.hpp"
#include "view/layoutmanager.hpp"
#include "view/menu.hpp"
#include "view/styling.hpp"
#include "view/widgets/input.hpp"
#include "view/widgets/scrollbar.hpp"
#include <memory>

MenuView::MenuView() : m_scroll_offset(0) {
  m_font = std::make_shared<opengl::Font>(
      "/home/falk/.fonts/MonoLisa/ttf/MonoLisa-Regular.ttf", 14);
  m_input = std::make_unique<Input>(m_font);
  m_scroll_bar = std::make_unique<ScrollBar>();
}

void MenuView::init(std::shared_ptr<opengl::Renderer> renderer,
                    std::shared_ptr<KakouneContentView> kakoune_content_view) {
  m_renderer = renderer;
  m_kakoune_content_view = kakoune_content_view;
}

void MenuView::render(const KakouneClient &kakoune_client, float width,
                      float height) {
  if (!kakoune_client.menu_visible &&
      kakoune_client.menu_style !=
          kakoune::MenuStyle::PROMPT) // Special case for Prompt.
    return;

  switch (kakoune_client.menu_style) {
  case kakoune::MenuStyle::INLINE:
    renderInlineStyle(kakoune_client, width, height);
    break;
  case kakoune::MenuStyle::PROMPT:
    renderPromptStyle(kakoune_client, width, height);
    break;
  }
}

void MenuView::renderPromptStyle(const KakouneClient &kakoune_client,
                                 float width, float height) {
  if (kakoune_client.status_line.atoms.size() < 1)
    return;

  static bool opened_before = false;
  if (!kakoune_client.menu_visible && !opened_before)
    return;
  opened_before = true;

  float menu_x = (width - PROMPT_WIDTH) / 2;
  float line_height = m_font->getLineHeight();

  float items_length = MAX_PROMPT_ITEMS < kakoune_client.menu_items.size()
                           ? MAX_PROMPT_ITEMS
                           : kakoune_client.menu_items.size();

  m_input->setContent(kakoune_client.status_line);

  float menu_height = 2 * SPACING_MEDIUM + m_input->height();
  if (kakoune_client.menu_visible) {
    menu_height += SPACING_MEDIUM + line_height * items_length;
  }

  LayoutManager layout(menu_x, PROMPT_Y_POSITION, PROMPT_WIDTH, menu_height);

  m_renderer->renderRectWithShadow(
      kakoune_client.menu_face.bg.toCoreColor(
          kakoune_client.window_default_face.bg, false),
      layout.current().x, layout.current().y, layout.current().width,
      layout.current().height, 15.0f);

  layout.pad(SPACING_MEDIUM);

  m_input->render(m_renderer, kakoune_client, layout);

  if (!kakoune_client.menu_visible)
    return;

  layout.gapY(SPACING_MEDIUM);

  auto items_layout = layout.copy();
  renderScrolledContent(kakoune_client, items_layout, MAX_PROMPT_ITEMS);

  if (kakoune_client.menu_items.size() > MAX_PROMPT_ITEMS) {
    m_scroll_bar->setValue(m_scroll_offset,
                           kakoune_client.menu_items.size() - MAX_PROMPT_ITEMS,
                           MAX_PROMPT_ITEMS);
    m_scroll_bar->render(
        m_renderer,
        kakoune_client.status_default_face.fg.toCoreColor(std::nullopt, true),
        layout);
  }
}

void MenuView::renderInlineStyle(const KakouneClient &kakoune_client,
                                 float width, float height) {
  auto anchor = kakoune_client.menu_anchor;

  float paddingX = 4.0f;
  float paddingY = 0.0f;

  int max_item_length = 0;
  for (const auto &item : kakoune_client.menu_items) {
    int item_length = item.size();

    if (item_length > max_item_length) {
      max_item_length = item_length;
    }
  }

  auto menu_position =
      m_kakoune_content_view->coordToPixels(kakoune_client.menu_anchor);
  float menu_x = menu_position.first,
        menu_y = menu_position.second +
                 m_kakoune_content_view->getCellHeight() + SPACING_MEDIUM;

  float menu_width = 2 * paddingX +
                     max_item_length * m_kakoune_content_view->getCellWidth() +
                     SPACING_MEDIUM + m_scroll_bar->width();
  float line_height = m_font->getLineHeight();
  float menu_height = 2 * paddingY + line_height * std::min((int)kakoune_client.menu_items.size(), MAX_INLINE_ITEMS);

  LayoutManager layout(menu_x, menu_y, menu_width, menu_height);

  m_renderer->renderRect(kakoune_client.menu_face.bg.toCoreColor(
                             kakoune_client.window_default_face.bg, false),
                         layout.current().x, layout.current().y,
                         layout.current().width, layout.current().height);

  layout.pad(paddingY, paddingX);

  auto items_layout = layout.copy();
  renderScrolledContent(kakoune_client, items_layout, MAX_INLINE_ITEMS);

  if (kakoune_client.menu_items.size() > MAX_INLINE_ITEMS) {
    m_scroll_bar->setValue(m_scroll_offset,
                           kakoune_client.menu_items.size() - MAX_INLINE_ITEMS,
                           MAX_INLINE_ITEMS);
    m_scroll_bar->render(
        m_renderer,
        kakoune_client.status_default_face.fg.toCoreColor(std::nullopt, true),
        layout);
  }
}

void MenuView::renderScrolledContent(const KakouneClient &kakoune_client,
                                     LayoutManager &layout, int max_items) {
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

    if (i == selected_index) {
      m_renderer->renderRect(kakoune_client.menu_selected_face.bg.toCoreColor(
                                 kakoune_client.window_default_face.bg, false),
                             layout.current().x - SPACING_MEDIUM, layout.current().y, layout.current().width + SPACING_MEDIUM * 2,
                             line_height);

      m_renderer->renderLine(*m_font, item, kakoune_client.menu_selected_face,
                             layout.current().x, layout.current().y);
    } else {
      m_renderer->renderLine(*m_font, item, kakoune_client.menu_face,
                             layout.current().x, layout.current().y);
    }
    layout.sliceTop(line_height);
  }
}
