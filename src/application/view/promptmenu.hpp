#ifndef VIEW_PROMPTMENU_HPP_INCLUDED
#define VIEW_PROMPTMENU_HPP_INCLUDED

#include "application/model/viewstate.hpp"
#include "application/observerlist.hpp"
#include "application/view/kakounecontentview.hpp"
#include "application/view/widgets/input.hpp"
#include "application/view/widgets/scrolledmenuitems.hpp"
#include "domain/mouse.hpp"
#include <functional>

class PromptMenuView
{
  public:
    PromptMenuView();

    void init(domain::Renderer *renderer, KakouneContentView *kakoune_content_view);

    void render(const RenderContext &render_context, MenuViewState &state, const domain::Menu &menu, int cursor_column);

    domain::MouseMoveResult handleMouseMove(float x, float y, const domain::Menu &menu);
    bool handleMouseButton(domain::MouseButtonEvent event, MenuViewState &state, const domain::Menu &menu);
    void handleMouseScroll(MenuViewState &state, int scroll_amount, const domain::Menu &menu);
    void ensureItemVisible(MenuViewState &state, int index);

    ObserverId onMouseButton(std::function<void(int)> callback);
    void removeObserver(ObserverId id);

    float x() const;
    float y() const;
    float width() const;
    float height() const;

    float scrolledItemsX() const;
    float scrolledItemsY() const;
    float scrolledItemsWidth() const;
    float scrolledItemsHeight() const;

  private:
    const float Y = 60.0f;
    const float WIDTH = 600.0f;
    const int MAX_VISIBLE_ITEMS = 25;

    float m_x;
    float m_height;

    KakouneContentView *m_kakoune_content_view;
    domain::Renderer *m_renderer;

    std::unique_ptr<Input> m_input;
    std::unique_ptr<ScrolledMenuItems> m_scrolled_menu_items;

    ObserverList<int> m_mouse_button_observers;

    std::optional<int> findItemAtPosition(float x, float y, const MenuViewState &state, const domain::Menu &menu);
};

#endif
