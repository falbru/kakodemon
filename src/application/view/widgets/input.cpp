#include "input.hpp"
#include "domain/editor.hpp"
#include "application/model/kakouneclient.hpp"
#include "application/view/glyphsequence.hpp"

Input::Input(domain::Font* font)
    : m_font(font) {}

void Input::setPrompt(const kakoune::Line& prompt) { m_prompt = prompt; }

void Input::setContent(const kakoune::Line& content) { m_content = content; }

void Input::render(domain::Renderer* renderer,
                   const KakouneClient& kakoune_client,
                   LayoutManager &layout) {
  auto input_layout = layout.sliceTop(height());

  renderer->renderRect(kakoune_client.menu_selected_face.bg.toCoreColor(
                           kakoune_client.window_default_face.bg, false),
                       input_layout.current().x,
                       input_layout.current().y,
                       input_layout.current().width,
                       input_layout.current().height);

  input_layout.border(BORDER);

  renderer->renderRect(kakoune_client.status_default_face.bg.toCoreColor(
                           kakoune_client.window_default_face.bg, false),
                       input_layout.current().x,
                       input_layout.current().y,
                       input_layout.current().width,
                       input_layout.current().height);

  input_layout.pad(PADDING);

  if (kakoune_client.status_line.prompt.size() > 0) {
      GlyphSequence prompt_glyph_seq(m_font, kakoune_client.status_line.prompt.toUTF8String());

      auto prompt_layout = input_layout.sliceLeft(prompt_glyph_seq.width());

      renderer->renderLine(m_font, kakoune_client.status_line.prompt,
                           kakoune_client.status_default_face,
                           prompt_layout.current().x, prompt_layout.current().y);
  }

  renderer->addBounds(input_layout.current().x, input_layout.current().y, input_layout.current().width, input_layout.current().height);


  int cursor_pos_column = std::get<domain::StatusLinePosition>(kakoune_client.cursor_pos).column;
  if (cursor_pos_column >= 0) {
      GlyphSequence content_glyph_seq(m_font, kakoune_client.status_line.content.toUTF8String());

      float glyphs_left = content_glyph_seq.substr(0, cursor_pos_column).width();
      if (content_glyph_seq.substr(0, cursor_pos_column).width() < m_offset_x) {
          m_offset_x = glyphs_left;
      }

      if (m_offset_x + input_layout.current().x > content_glyph_seq.width()) {
          m_offset_x = content_glyph_seq.width() - input_layout.current().x;
          if (m_offset_x < 0) m_offset_x = 0;
      }

      float glyphs_with_cursor = content_glyph_seq.substr(0, cursor_pos_column+1).width();
      if (glyphs_with_cursor - m_offset_x > input_layout.current().width) {
          m_offset_x = glyphs_with_cursor - input_layout.current().width;
      }
  }else {
      m_offset_x = 0;
  }

  renderer->renderLine(m_font, kakoune_client.status_line.content,
                       kakoune_client.status_default_face,
                       input_layout.current().x - m_offset_x, input_layout.current().y);
  renderer->popBounds();
}

float Input::width() const {
  return BORDER * 2 + PADDING * 2 +
         m_font->width(m_content.toUTF8String());
}

float Input::height() const {
  return BORDER * 2 + PADDING * 2 + m_font->getLineHeight();
}
