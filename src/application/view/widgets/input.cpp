#include "input.hpp"
#include "domain/glyphlinesbuilder.hpp"
#include "domain/line.hpp"

Input::Input() {}

void Input::render(domain::Renderer *renderer, const RenderContext &render_context, domain::Font* font, const domain::StatusLine &input, const domain::Face &face, int cursor_column, InputViewState &state, LayoutManager &layout) {
  auto input_layout = layout.sliceTop(height(font));

  if (input.getPrompt().size() > 0) {
      auto prompt_layout = input_layout.sliceLeft(domain::GlyphLinesBuilder::build(input.getPrompt(), font, render_context.font_manager).width());

      renderer->renderLine(render_context.textConfig(font), input.getPrompt(), face, prompt_layout.current().x, prompt_layout.current().y);
  }

  renderer->addBounds(input_layout.current().x, input_layout.current().y, input_layout.current().width, input_layout.current().height);

  if (cursor_column >= 0) {
      domain::Line content_line = input.getContent();

      float cursor_x = domain::GlyphLinesBuilder::build(content_line.slice(0, cursor_column), font, render_context.font_manager).width();
      if (cursor_x < state.scroll_offset) {
          state.scroll_offset = cursor_x;
      }

      float content_width = domain::GlyphLinesBuilder::build(content_line, font, render_context.font_manager).width();
      float cursor_space = font->getGlyphMetrics(' ').advance;
      if (state.scroll_offset + input_layout.current().width > content_width + cursor_space) {
          state.scroll_offset = std::max(0.0f, content_width + cursor_space - input_layout.current().width);
      }

      float cursor_x_right = domain::GlyphLinesBuilder::build(content_line.slice(0, cursor_column + 1), font, render_context.font_manager).width();
      if (cursor_x_right - state.scroll_offset > input_layout.current().width) {
          state.scroll_offset = cursor_x_right - input_layout.current().width;
      }
  } else {
      state.scroll_offset = 0;
  }

  renderer->renderLine(render_context.textConfig(font), input.getContent(), face, input_layout.current().x - state.scroll_offset, input_layout.current().y);
  renderer->popBounds();
}

float Input::height(domain::Font* font) const {
  return font->getLineHeight();
}
