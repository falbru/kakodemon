#include "input.hpp"
#include "domain/editor.hpp"
#include "application/model/kakouneclient.hpp"
#include "domain/glyphlinesbuilder.hpp"
#include "domain/line.hpp"

Input::Input() {}

void Input::setPrompt(const domain::Line& prompt) { m_prompt = prompt; }

void Input::setContent(const domain::Line& content) { m_content = content; }

void Input::render(domain::Renderer *renderer, domain::Font* font, domain::FontManager* font_manager, const KakouneClient &kakoune_client, LayoutManager &layout) {
  auto input_layout = layout.sliceTop(height(font));

  renderer->renderRect(kakoune_client.state.menu->getSelectedFace().getBg(kakoune_client.state.default_face),
                       input_layout.current().x,
                       input_layout.current().y,
                       input_layout.current().width,
                       input_layout.current().height);

  input_layout.border(BORDER);

  renderer->renderRect(kakoune_client.state.mode_line.getDefaultFace().getBg(
                           kakoune_client.state.default_face),
                       input_layout.current().x,
                       input_layout.current().y,
                       input_layout.current().width,
                       input_layout.current().height);

  input_layout.pad(PADDING);

  if (kakoune_client.state.menu->getInput().getPrompt().size() > 0) {
      auto prompt_layout = input_layout.sliceLeft(domain::GlyphLinesBuilder::build(kakoune_client.state.menu->getInput().getPrompt(), font, font_manager).width());

      renderer->renderLine(font, font_manager, kakoune_client.state.menu->getInput().getPrompt(),
                           kakoune_client.state.mode_line.getDefaultFace(),
                           prompt_layout.current().x, prompt_layout.current().y);
  }

  renderer->addBounds(input_layout.current().x, input_layout.current().y, input_layout.current().width, input_layout.current().height);


  int cursor_pos_column = std::get<domain::StatusLinePosition>(kakoune_client.state.cursor_position).column;
  if (cursor_pos_column >= 0) {
      domain::Line content_line = kakoune_client.state.menu->getInput().getContent();

      float cursor_x = domain::GlyphLinesBuilder::build(content_line.slice(0, cursor_pos_column), font, font_manager).width();
      if (cursor_x < m_offset_x) {
          m_offset_x = cursor_x;
      }

      float content_width = domain::GlyphLinesBuilder::build(content_line, font, font_manager).width();
      if (m_offset_x + input_layout.current().x > content_width) {
          m_offset_x = std::max(0.0f, content_width - input_layout.current().x);
      }

      float cursor_x_right = domain::GlyphLinesBuilder::build(content_line.slice(0, cursor_pos_column + 1), font, font_manager).width();
      if (cursor_x_right - m_offset_x > input_layout.current().width) {
          m_offset_x = cursor_x_right - input_layout.current().width;
      }
  }else {
      m_offset_x = 0;
  }

  renderer->renderLine(font, font_manager, kakoune_client.state.menu->getInput().getContent(),
                       kakoune_client.state.mode_line.getDefaultFace(),
                       input_layout.current().x - m_offset_x, input_layout.current().y);
  renderer->popBounds();
}

float Input::height(domain::Font* font) const {
  return BORDER * 2 + PADDING * 2 + font->getLineHeight();
}
