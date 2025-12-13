#ifndef STATUSBAR_HPP_INCLUDED
#define STATUSBAR_HPP_INCLUDED

#include "domain/ports/renderer.hpp"

class StatusBarView
{
  public:
    StatusBarView();

    void init(domain::Renderer *renderer);

    void render(domain::Font *font, const kakoune::Line &mode, const kakoune::Face &default_face, float width,
                float height);

    int getCellWidth(domain::Font *font);
    int getCellHeight(domain::Font *font);

  private:
    domain::Renderer *m_renderer;
};

#endif // statusbar_hpp_INCLUDED
