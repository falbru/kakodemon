#ifndef VIEW_SEARCHMENU_HPP_INCLUDED
#define VIEW_SEARCHMENU_HPP_INCLUDED

#include "application/model/viewstate.hpp"
#include "application/view/rendercontext.hpp"
#include "application/view/widgets/input.hpp"
#include "application/view/widgets/scrolledmenuitems.hpp"
#include "domain/menu.hpp"
#include "domain/mouse.hpp"
#include "domain/ports/renderer.hpp"

class SearchMenuView
{
  public:
    SearchMenuView();

    void init(domain::Renderer *renderer);

    void render(const RenderContext &render_context, MenuViewState &state, const domain::Menu &menu, int cursor_column);

    domain::MouseMoveResult onMouseMove(float x, float y, const domain::Menu &menu);
    std::optional<int> findItemAtPosition(float x, float y, const MenuViewState &state, const domain::Menu &menu);
    void onMouseScroll(MenuViewState &state, int scroll_amount, const domain::Menu &menu);
    void ensureItemVisible(MenuViewState &state, int index);

    float x() const;
    float y() const;
    float width() const;
    float height() const;

    float scrolledItemsX() const;
    float scrolledItemsY() const;
    float scrolledItemsWidth() const;
    float scrolledItemsHeight() const;

  private:
    const float WIDTH = 600.0f;
    const int MAX_VISIBLE_ITEMS = 10;

    float m_x;
    float m_height;

    domain::Renderer *m_renderer;

    std::unique_ptr<Input> m_input;
    std::unique_ptr<ScrolledMenuItems> m_scrolled_menu_items;
};

#endif
