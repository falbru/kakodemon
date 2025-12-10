#ifndef VIEW_INLINEMENU_HPP_INCLUDED
#define VIEW_INLINEMENU_HPP_INCLUDED

#include "model/kakouneclient.hpp"
#include "view/kakounecontentview.hpp"
#include "view/widgets/scrolledmenuitems.hpp"
#include <memory>

class InlineMenuView
{
  public:
    InlineMenuView();

    void init(Renderer *renderer, KakouneContentView *kakoune_content_view);

    void render(Font *font, const KakouneClient &kakoune_client, float width, float height);

    float x() const;
    float y() const;
    float width() const;
    float height() const;

  private:
    int m_scroll_offset;

    const float MAX_MENU_WIDTH = 700;
    const int MAX_VISIBLE_ITEMS = 12;

    float m_x;
    float m_y;
    float m_width;
    float m_height;

    KakouneContentView *m_kakoune_content_view;
    Renderer *m_renderer;

    std::unique_ptr<ScrolledMenuItems> m_scrolled_menu_items;
};

#endif
