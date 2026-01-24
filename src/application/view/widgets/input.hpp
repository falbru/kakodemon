#ifndef VIEW_WIDGETS_INPUT_HPP_INCLUDED
#define VIEW_WIDGETS_INPUT_HPP_INCLUDED

#include "application/view/layoutmanager.hpp"
#include "application/view/rendercontext.hpp"
#include "domain/face.hpp"
#include "domain/ports/renderer.hpp"
#include "domain/statusline.hpp"

class Input
{
  public:
    Input();

    void render(domain::Renderer *renderer, const RenderContext &render_context, domain::Font *font,
                const domain::StatusLine &input, const domain::Face &face, int cursor_column, LayoutManager &layout);

    float height(domain::Font *font) const;

  private:
    float m_offset_x = 0.0f;
};

#endif
