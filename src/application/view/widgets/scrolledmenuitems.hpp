#ifndef VIEW_WIDGETS_SCROLLEDMENUITEMS_HPP_INCLUDED
#define VIEW_WIDGETS_SCROLLEDMENUITEMS_HPP_INCLUDED

#include "application/model/viewstate.hpp"
#include "application/view/layoutmanager.hpp"
#include "application/view/rendercontext.hpp"
#include "application/view/widgets/scrollbar.hpp"
#include "domain/menu.hpp"
#include "domain/ports/renderer.hpp"
#include <memory>

class ScrolledMenuItems
{
  public:
    ScrolledMenuItems(int max_items);

    void render(domain::Renderer *renderer, const RenderContext &render_context, MenuViewState &state,
                const domain::MenuItems &menu_items, const domain::RGBAColor &scrollbar_color, LayoutManager &layout);

    float width() const;
    float height() const;
    float x() const;
    float y() const;

    std::optional<int> findItemAtPosition(float x, float y, const MenuViewState &state,
                                          const domain::MenuItems &menu_items);

    void scroll(MenuViewState &state, int amount, int total_items);
    void ensureItemVisible(MenuViewState &state, int index);

    float getRightPadding(int total_items) const;

  private:
    int m_max_visible_items = 0;

    float m_x;
    float m_y;
    float m_width;
    float m_height;

    std::unique_ptr<ScrollBar> m_scroll_bar;
};

#endif
