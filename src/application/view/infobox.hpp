#ifndef VIEW_INFOBOX_HPP_INCLUDED
#define VIEW_INFOBOX_HPP_INCLUDED
#include "application/model/viewstate.hpp"
#include "application/view/kakounecontentview.hpp"
#include "application/view/multistyledmenu.hpp"
#include "application/view/rendercontext.hpp"
#include "application/view/statusbar.hpp"
#include "application/view/widgets/scrollbar.hpp"
#include "domain/editor.hpp"
#include "domain/geometry.hpp"
#include "domain/infobox.hpp"
#include <memory>

enum class PlacementDirection
{
    ABOVE,
    BELOW,
    LEFT,
    RIGHT,
};

struct AnchorPlacement
{
    domain::Rectangle anchor;
    std::vector<PlacementDirection> preferred_directions;
};

struct FixedPlacement
{
    domain::Rectangle placement;
};

struct PlacementConfig
{
    std::variant<AnchorPlacement, FixedPlacement> placement;
    bool absolute_position;
};

class InfoBoxView
{
  public:
    InfoBoxView();
    void init(domain::Renderer *renderer, MultiStyledMenuView *multi_styled_menu,
              KakouneContentView *kakoune_content_view, StatusBarView *status_bar_view);

    void render(const RenderContext &render_context, InfoBoxViewState &state, const domain::InfoBox &info_box,
                const domain::CursorPosition &cursor_position, const domain::IVec2 &cursor_origin,
                const domain::Rectangle &content_bounds);

    void handleMouseScroll(InfoBoxViewState &state, int scroll_amount);

    float x() const;
    float y() const;
    float width() const;
    float height() const;

  private:
    PlacementConfig placementConfigByInfoBoxStyle(const RenderContext &render_context, InfoBoxViewState &state,
                                                  const domain::InfoBox &info_box,
                                                  const domain::CursorPosition &cursor_position,
                                                  const domain::IVec2 &cursor_origin, int info_box_width,
                                                  int info_box_height);
    std::optional<domain::Rectangle> placeInfoBox(const RenderContext &render_context, InfoBoxViewState &state,
                                                  const domain::InfoBox &info_box,
                                                  const domain::CursorPosition &cursor_position,
                                                  const domain::IVec2 &cursor_origin,
                                                  const domain::Rectangle &content_bounds, int info_box_width,
                                                  int info_box_height);
    std::optional<domain::Rectangle> placeWithoutOverlap(const domain::Rectangle &bounds,
                                                         const domain::IVec2 &fitted_rectangle_size,
                                                         const domain::Rectangle &anchor,
                                                         const std::vector<PlacementDirection> &preferred_directions,
                                                         const std::vector<domain::Rectangle> &obstacles);
    std::vector<domain::Rectangle> findNonOverlappingPlacements(const domain::Rectangle &bounds,
                                                                const domain::IVec2 &fitted_rectangle_size,
                                                                const domain::IVec2 &start_position,
                                                                const std::vector<domain::Rectangle> &obstacles);

    const int MIN_WIDTH = 150;
    const int MAX_WIDTH = 1000;
    const int MAX_HEIGHT = 600;
    domain::Renderer *m_renderer;
    KakouneContentView *m_kakoune_content_view;
    MultiStyledMenuView *m_multi_styled_menu;
    StatusBarView *m_status_bar_view;

    float m_x = 0.0f;
    float m_y = 0.0f;
    float m_width = 0.0f;
    float m_height = 0.0f;
    std::unique_ptr<ScrollBar> m_scroll_bar;
};

#endif // VIEW_INFOBOX_HPP_INCLUDED
