#ifndef VIEW_PROMPTMENU_HPP_INCLUDED
#define VIEW_PROMPTMENU_HPP_INCLUDED

#include "application/model/kakouneclient.hpp"
#include "application/view/kakounecontentview.hpp"
#include "application/view/widgets/input.hpp"
#include "application/view/widgets/scrolledmenuitems.hpp"

class PromptMenuView
{
  public:
    PromptMenuView();

    void init(Renderer *renderer, KakouneContentView *kakoune_content_view);

    void render(Font *font, const KakouneClient &kakoune_client, float width, float height);

    float x() const;
    float y() const;
    float width() const;
    float height() const;

  private:
    int m_scroll_offset;

    const float Y = 60.0f;
    const float WIDTH = 600.0f;
    const int MAX_VISIBLE_ITEMS = 25;

    float m_x;
    float m_height;

    KakouneContentView *m_kakoune_content_view;
    Renderer *m_renderer;

    std::unique_ptr<Input> m_input;
    std::unique_ptr<ScrolledMenuItems> m_scrolled_menu_items;
};

#endif
