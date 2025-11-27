#ifndef VIEW_PROMPTMENU_HPP_INCLUDED
#define VIEW_PROMPTMENU_HPP_INCLUDED

#include "model/kakouneclient.hpp"
#include "opengl/renderer.hpp"
#include "view/kakounecontentview.hpp"
#include "view/widgets/input.hpp"
#include "view/widgets/scrolledmenuitems.hpp"

class PromptMenuView
{
  public:
    PromptMenuView();

    void init(std::shared_ptr<opengl::Renderer> renderer, std::shared_ptr<KakouneContentView> kakoune_content_view);

    void render(const KakouneClient &kakoune_client, float width, float height);

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

    std::shared_ptr<KakouneContentView> m_kakoune_content_view;
    std::shared_ptr<opengl::Renderer> m_renderer;
    std::shared_ptr<opengl::Font> m_font;

    std::unique_ptr<Input> m_input;
    std::unique_ptr<ScrolledMenuItems> m_scrolled_menu_items;
};

#endif
