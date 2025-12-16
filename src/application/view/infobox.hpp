#ifndef VIEW_INFOBOX_HPP_INCLUDED
#define VIEW_INFOBOX_HPP_INCLUDED
#include "application/controller/menucontroller.hpp"
#include "application/model/kakouneclient.hpp"
#include "application/model/uioptions.hpp"
#include "application/view/kakounecontentview.hpp"
#include "domain/geometry.hpp"
#include "domain/lines.hpp"

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
    domain::Lines content;
    domain::Rectangle bounds;
};

class InfoBoxView
{
  public:
    InfoBoxView();
    void init(domain::Renderer *renderer, MenuController *menu_controller, KakouneContentView *kakoune_content_view);

    std::pair<float, float> calculateInfoBoxPosition(const domain::Rectangle &anchor, float info_box_width,
                                                     float info_box_height, float viewport_width, float viewport_height,
                                                     PlacementDirection direction, CrossAxisAlignment alignment) const;

    std::optional<Placement> tryPlaceInfoBox(PlacementDirection direction, CrossAxisAlignment alignment,
                                             const domain::Lines &content, const domain::Rectangle &anchor,
                                             float layout_width, float layout_height, domain::Font *font,
                                             const domain::Rectangle &menu_rectangle);

    void render(const KakouneClient *kakoune_client, const UIOptions &ui_options, float width, float height);

    float x() const;
    float y() const;
    float width() const;
    float height() const;

  private:
    const float MIN_WIDTH = 150.0f;
    const float MAX_WIDTH = 1000.0f;
    domain::Renderer *m_renderer;
    KakouneContentView *m_kakoune_content_view;
    MenuController *m_menu_controller;

    float m_x = 0.0f;
    float m_y = 0.0f;
    float m_width = 0.0f;
    float m_height = 0.0f;
};

#endif // VIEW_INFOBOX_HPP_INCLUDED
