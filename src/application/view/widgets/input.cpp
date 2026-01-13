#include "input.hpp"
#include "domain/glyphlinesbuilder.hpp"
#include "domain/line.hpp"

Input::Input() {}

void Input::setPrompt(const domain::Line& prompt) { m_prompt = prompt; }

void Input::setContent(const domain::Line& content) { m_content = content; }

void Input::render(domain::Renderer *renderer, const RenderContext &render_context, const domain::StatusLine &input, const domain::Face &face, int cursor_column, LayoutManager &layout) {
  domain::Font* font = render_context.ui_options.font_menu;
  auto input_layout = layout.sliceTop(height(font));

  renderer->renderRect(face.getBg(render_context.default_face, render_context.ui_options.color_overrides),
                       input_layout.current().x,
                       input_layout.current().y,
                       input_layout.current().width,
                       input_layout.current().height);

  input_layout.pad(PADDING);

  if (input.getPrompt().size() > 0) {
      auto prompt_layout = input_layout.sliceLeft(domain::GlyphLinesBuilder::build(input.getPrompt(), font, render_context.font_manager).width());

      renderer->renderLine(render_context.textConfig(font), input.getPrompt(), face, prompt_layout.current().x, prompt_layout.current().y);
  }

  renderer->addBounds(input_layout.current().x, input_layout.current().y, input_layout.current().width, input_layout.current().height);

  if (cursor_column >= 0) {
      domain::Line content_line = input.getContent();

      float cursor_x = domain::GlyphLinesBuilder::build(content_line.slice(0, cursor_column), font, render_context.font_manager).width();
      if (cursor_x < m_offset_x) {
          m_offset_x = cursor_x;
      }

      float content_width = domain::GlyphLinesBuilder::build(content_line, font, render_context.font_manager).width();
      float cursor_space = font->getGlyphMetrics(' ').advance;
      if (m_offset_x + input_layout.current().width > content_width + cursor_space) {
          m_offset_x = std::max(0.0f, content_width + cursor_space - input_layout.current().width);
      }

      float cursor_x_right = domain::GlyphLinesBuilder::build(content_line.slice(0, cursor_column + 1), font, render_context.font_manager).width();
      if (cursor_x_right - m_offset_x > input_layout.current().width) {
          m_offset_x = cursor_x_right - input_layout.current().width;
      }
  } else {
      m_offset_x = 0;
  }

  renderer->renderLine(render_context.textConfig(font), input.getContent(), face, input_layout.current().x - m_offset_x, input_layout.current().y);
  renderer->popBounds();
}

float Input::height(domain::Font* font) const {
  return PADDING * 2 + font->getLineHeight();
}
