#ifndef VIEW_INFOBOX_HPP_INCLUDED
#define VIEW_INFOBOX_HPP_INCLUDED

#include "controller/menucontroller.hpp"
#include "model/kakouneclient.hpp"
#include "opengl/renderer.hpp"
#include "view/kakounecontentview.hpp"

class InfoBoxView
{
  public:
    InfoBoxView();

    void init(std::shared_ptr<opengl::Renderer> renderer, std::shared_ptr<MenuController> menu_controller, std::shared_ptr<KakouneContentView> kakoune_content_view);

    void render(const KakouneClient &kakoune_client, float width, float height);

  private:
    const float MAX_WIDTH = 1000.0f;

    std::shared_ptr<opengl::Renderer> m_renderer;
    std::shared_ptr<KakouneContentView> m_kakoune_content_view;
    std::shared_ptr<opengl::Font> m_font;
    std::shared_ptr<MenuController> m_menu_controller;
};
#endif // infobox_hpp_INCLUDED
