#ifndef STATUSBAR_HPP_INCLUDED
#define STATUSBAR_HPP_INCLUDED

#include "application/model/viewstate.hpp"
#include "application/view/rendercontext.hpp"
#include "application/view/widgets/input.hpp"
#include "domain/editor.hpp"
#include "domain/geometry.hpp"
#include "domain/modeline.hpp"
#include "domain/ports/font.hpp"
#include "domain/ports/renderer.hpp"

class StatusBarView
{
  public:
    StatusBarView();

    void init(domain::Renderer *renderer);

    void render(const RenderContext &render_context, InputViewState &state, const domain::ModeLine &mode_line,
                domain::CursorPosition cursor_position, const domain::Rectangle &bounds);

    float height(domain::Font *font);

  private:
    domain::Renderer *m_renderer;
    Input m_input_widget;
};

#endif // statusbar_hpp_INCLUDED
