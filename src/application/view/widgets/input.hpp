#ifndef VIEW_WIDGETS_INPUT_HPP_INCLUDED
#define VIEW_WIDGETS_INPUT_HPP_INCLUDED

#include "application/view/layoutmanager.hpp"
#include "application/view/rendercontext.hpp"
#include "application/view/styling.hpp"
#include "domain/face.hpp"
#include "domain/ports/renderer.hpp"
#include "domain/statusline.hpp"

class Input
{
  public:
    Input();

    void setPrompt(const domain::Line &prompt);
    void setContent(const domain::Line &content);

    void render(domain::Renderer *renderer, const RenderContext &render_context, const domain::StatusLine &input,
                const domain::Face &face, int cursor_column, LayoutManager &layout);

    float height(domain::Font *font) const;

  private:
    float m_offset_x = 0.0f;

    domain::Line m_prompt;
    domain::Line m_content;

    const float BORDER = 1.0f;
    const float PADDING = SPACING_MEDIUM;
};

#endif
