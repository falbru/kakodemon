#ifndef STATUSBAR_HPP_INCLUDED
#define STATUSBAR_HPP_INCLUDED

#include "application/view/rendercontext.hpp"
#include "domain/modeline.hpp"
#include "domain/ports/font.hpp"
#include "domain/ports/renderer.hpp"

class StatusBarView
{
  public:
    StatusBarView();

    void init(domain::Renderer *renderer);

    void render(const RenderContext &render_context, const domain::ModeLine &mode_line);

    float height(domain::Font *font);

  private:
    domain::Renderer *m_renderer;
};

#endif // statusbar_hpp_INCLUDED
