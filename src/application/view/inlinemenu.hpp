#ifndef VIEW_INLINEMENU_HPP_INCLUDED
#define VIEW_INLINEMENU_HPP_INCLUDED

#include "application/view/kakounecontentview.hpp"
#include "application/view/rendercontext.hpp"
#include "application/view/widgets/scrolledmenuitems.hpp"
#include "domain/menu.hpp"
#include "domain/mouse.hpp"
#include <memory>

class InlineMenuView
{
  public:
    InlineMenuView();

    void init(domain::Renderer *renderer, KakouneContentView *kakoune_content_view);

    void render(const RenderContext &render_context, const domain::Menu &menu);

    domain::MouseMoveResult onMouseMove(float x, float y, const domain::Menu &menu);
    std::optional<int> findItemAtPosition(float x, float y, const domain::Menu &menu);
    void onMouseScroll(int scroll_amount, const domain::Menu &menu);
    void ensureItemVisible(int index);

    float x() const;
    float y() const;
    float width() const;
    float height() const;

    float scrolledItemsX() const;
    float scrolledItemsY() const;
    float scrolledItemsWidth() const;
    float scrolledItemsHeight() const;

  private:
    int m_scroll_offset;

    const float MAX_MENU_WIDTH = 700;
    const int MAX_VISIBLE_ITEMS = 12;

    float m_x;
    float m_y;
    float m_width;
    float m_height;

    KakouneContentView *m_kakoune_content_view;
    domain::Renderer *m_renderer;

    std::unique_ptr<ScrolledMenuItems> m_scrolled_menu_items;
};

#endif
