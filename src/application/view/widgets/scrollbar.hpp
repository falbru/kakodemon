#ifndef VIEW_WIDGETS_SCROLLBAR_HPP_INCLUDED
#define VIEW_WIDGETS_SCROLLBAR_HPP_INCLUDED

#include "application/view/layoutmanager.hpp"
#include "domain/ports/renderer.hpp"

class ScrollBar
{
  public:
    ScrollBar();

    void setValue(float value, float max_value, float visible_range);

    void render(domain::Renderer *renderer, domain::RGBAColor color, LayoutManager &layout) const;

    float width() const;

  private:
    float m_value, m_max_value, m_visible_range;

    const float WIDTH = 5.0f;
};

#endif
