#ifndef VIEW_SEARCHMENU_HPP_INCLUDED
#define VIEW_SEARCHMENU_HPP_INCLUDED

#include "application/model/kakouneclient.hpp"
#include "application/view/kakounecontentview.hpp"
#include "application/view/widgets/input.hpp"
#include "application/view/widgets/scrolledmenuitems.hpp"
#include "domain/mouse.hpp"
#include "domain/ports/renderer.hpp"

class SearchMenuView
{
  public:
    SearchMenuView();

    void init(domain::Renderer *renderer);

    void render(domain::Font *font, const KakouneClient &kakoune_client, float width, float height);

    domain::MouseMoveResult onMouseMove(float x, float y);

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

    const float WIDTH = 600.0f;
    const int MAX_VISIBLE_ITEMS = 25;

    float m_x;
    float m_height;

    KakouneContentView *m_kakoune_content_view;
    domain::Renderer *m_renderer;

    std::unique_ptr<Input> m_input;
    std::unique_ptr<ScrolledMenuItems> m_scrolled_menu_items;
};

#endif
