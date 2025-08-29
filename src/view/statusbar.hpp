#ifndef STATUSBAR_HPP_INCLUDED
#define STATUSBAR_HPP_INCLUDED

#include "opengl/renderer.hpp"
#include <memory>

class StatusBarView
{
  public:
    StatusBarView();

    void init(std::shared_ptr<opengl::Renderer> renderer);

    void render(const kakoune::Line &status, const kakoune::Line &mode, const kakoune::Face &default_face, float width,
                float height);

    int getCellWidth();
    int getCellHeight();

  private:
    std::shared_ptr<opengl::Renderer> m_renderer;
    opengl::Font m_font;
};

#endif // statusbar_hpp_INCLUDED
