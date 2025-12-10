#ifndef VIEW_INFOBOX_HPP_INCLUDED
#define VIEW_INFOBOX_HPP_INCLUDED
#include "controller/menucontroller.hpp"
#include "model/kakouneclient.hpp"
#include "model/uioptions.hpp"
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
    void init(Renderer *renderer, MenuController *menu_controller, KakouneContentView *kakoune_content_view);

    std::pair<float, float> calculateInfoBoxPosition(const Rectangle &anchor, float info_box_width,
                                                     float info_box_height, float viewport_width, float viewport_height,
                                                     PlacementDirection direction, CrossAxisAlignment alignment) const;

    std::pair<std::vector<kakoune::Line>, std::pair<float, float>> calculateWrappedContent(
        const std::vector<kakoune::Line> &input_lines, float max_width, Font *font) const;

    std::optional<Placement> tryPlaceInfoBox(PlacementDirection direction, CrossAxisAlignment alignment,
                                             const std::vector<kakoune::Line> &content, const Rectangle &anchor,
                                             float layout_width, float layout_height, Font *font);

    void render(const KakouneClient &kakoune_client, const UIOptions &ui_options, float width, float height);

  private:
    const float MIN_WIDTH = 150.0f;
    const float MAX_WIDTH = 1000.0f;
    Renderer *m_renderer;
    KakouneContentView *m_kakoune_content_view;
    MenuController *m_menu_controller;
};

#endif // VIEW_INFOBOX_HPP_INCLUDED
