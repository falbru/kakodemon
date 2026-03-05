#ifndef PANEBORDERVIEW_HPP_INCLUDED
#define PANEBORDERVIEW_HPP_INCLUDED

#include "application/model/panelayout.hpp"
#include "domain/ports/renderer.hpp"
#include <vector>

class PaneBorderView
{
  public:
    void init(domain::Renderer *renderer);
    void render(const std::vector<Pane> &panes, LayoutType layout_type);

  private:
    domain::Renderer *m_renderer;
};

#endif
