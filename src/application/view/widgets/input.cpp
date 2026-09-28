#include "input.hpp"
#include "domain/glyphresolver.hpp"
#include "domain/line.hpp"
#include "domain/renderline.hpp"

Input::Input()
{
}

void Input::render(domain::Renderer *renderer, const RenderContext &render_context, domain::Font *font,
                   const domain::StatusLine &input, const domain::Face &face, int cursor_column, InputViewState &state,
                   LayoutManager &layout)
{
    domain::GlyphResolver glyph_resolver(font, render_context.font_manager);
    auto input_layout = layout.sliceTop(height(font));

    if (input.getPrompt().size() > 0)
    {
        domain::RenderLine prompt_render_line(input.getPrompt(), glyph_resolver);
        auto prompt_layout = input_layout.sliceLeft(prompt_render_line.width());

        renderer->renderLine(render_context.textConfig(font), prompt_render_line, face, prompt_layout.current().x,
                             prompt_layout.current().y);
    }

    renderer->addBounds(input_layout.current().x, input_layout.current().y, input_layout.current().width,
                        input_layout.current().height);

    if (cursor_column >= 0)
    {
        domain::Line content_line = input.getContent();

        domain::RenderLine content_partial_render_line(content_line.slice(0, cursor_column), glyph_resolver);
        float cursor_x = content_partial_render_line.width();
        if (cursor_x < state.scroll_offset)
        {
            state.scroll_offset = cursor_x;
        }

        domain::RenderLine content_full_render_line(content_line, glyph_resolver);
        float content_width = content_full_render_line.width();
        float cursor_space = font->getGlyphMetrics(' ').advance;
        if (state.scroll_offset + input_layout.current().width > content_width + cursor_space)
        {
            state.scroll_offset = std::max(0.0f, content_width + cursor_space - input_layout.current().width);
        }

        domain::RenderLine cursor_right_render_line(content_line.slice(0, cursor_column + 1), glyph_resolver);
        float cursor_x_right = cursor_right_render_line.width();
        if (cursor_x_right - state.scroll_offset > input_layout.current().width)
        {
            state.scroll_offset = cursor_x_right - input_layout.current().width;
        }
    }
    else
    {
        state.scroll_offset = 0;
    }

    domain::RenderLine content_render_line(input.getContent(), glyph_resolver);
    renderer->renderLine(render_context.textConfig(font), content_render_line, face,
                         input_layout.current().x - state.scroll_offset, input_layout.current().y);
    renderer->popBounds();
}

float Input::height(domain::Font *font) const
{
    return font->getLineHeight();
}
