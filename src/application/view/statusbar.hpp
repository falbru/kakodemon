#ifndef STATUSBAR_HPP_INCLUDED
#define STATUSBAR_HPP_INCLUDED

#include "domain/ports/renderer.hpp"

class StatusBarView
{
  public:
    StatusBarView();

    void init(Renderer *renderer);

    void render(Font *font, const kakoune::Line &mode, const kakoune::Face &default_face, float width, float height);

    int getCellWidth(Font *font);
    int getCellHeight(Font *font);

  private:
    Renderer *m_renderer;
};

#endif // statusbar_hpp_INCLUDED
