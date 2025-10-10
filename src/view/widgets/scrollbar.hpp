#ifndef VIEW_WIDGETS_SCROLLBAR_HPP_INCLUDED
#define VIEW_WIDGETS_SCROLLBAR_HPP_INCLUDED

#include "opengl/renderer.hpp"
#include "view/layoutmanager.hpp"

class ScrollBar
{
  public:
    ScrollBar();

    void setValue(float value, float max_value, float visible_range);

    void render(std::shared_ptr<opengl::Renderer> renderer, core::Color color, LayoutManager &layout) const;

    float width() const;

  private:
    float m_value, m_max_value, m_visible_range;

    const float WIDTH = 5.0f;
};

#endif
