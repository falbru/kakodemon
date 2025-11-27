#ifndef VIEW_INFOBOX_HPP_INCLUDED
#define VIEW_INFOBOX_HPP_INCLUDED
#include "controller/menucontroller.hpp"
#include "model/kakouneclient.hpp"
#include "opengl/renderer.hpp"
#include "view/kakounecontentview.hpp"

enum class PlacementDirection
{
    ABOVE,
    BELOW,
    LEFT,
    RIGHT,
    CENTER,
    LEFT_WRAPPED,
    RIGHT_WRAPPED,
    FULL
};

enum class CrossAxisAlignment
{
    START,
    CENTER,
    END
};

struct Placement
{
    std::vector<kakoune::Line> content;
    Rectangle bounds;
};

class InfoBoxView
{
  public:
    InfoBoxView();
    void init(std::shared_ptr<opengl::Renderer> renderer, std::shared_ptr<MenuController> menu_controller,
              std::shared_ptr<KakouneContentView> kakoune_content_view);

    std::pair<float, float> calculateInfoBoxPosition(const Rectangle &anchor, float info_box_width,
                                                     float info_box_height, float viewport_width, float viewport_height,
                                                     PlacementDirection direction, CrossAxisAlignment alignment) const;

    std::pair<std::vector<kakoune::Line>, std::pair<float, float>> calculateWrappedContent(
        const std::vector<kakoune::Line> &input_lines, float max_width) const;

    std::optional<Placement> tryPlaceInfoBox(PlacementDirection direction, const std::vector<kakoune::Line> &content,
                                             const Rectangle &anchor, float layout_width, float layout_height);

    void render(const KakouneClient &kakoune_client, float width, float height);

  private:
    const float MIN_WIDTH = 150.0f;
    const float MAX_WIDTH = 1000.0f;
    std::shared_ptr<opengl::Renderer> m_renderer;
    std::shared_ptr<KakouneContentView> m_kakoune_content_view;
    std::shared_ptr<opengl::Font> m_font;
    std::shared_ptr<MenuController> m_menu_controller;
};

#endif // VIEW_INFOBOX_HPP_INCLUDED
