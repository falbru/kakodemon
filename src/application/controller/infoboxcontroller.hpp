#ifndef CONTROLLER_INFOBOXCONTROLLER_HPP_INCLUDED
#define CONTROLLER_INFOBOXCONTROLLER_HPP_INCLUDED

#include "application/controller/editorcontroller.hpp"
#include "application/model/kakouneclient.hpp"
#include "application/model/panelayout.hpp"
#include "application/view/infobox.hpp"
#include "application/window.hpp"
#include <functional>

class InfoBoxController
{
  public:
    InfoBoxController();

    void init(KakouneClient **focused_client, PaneLayout *layout_controller, Window *window,
              domain::FontManager *font_manager, InfoBoxView *info_box_view, std::function<void()> mark_dirty);

    void render();

    void onMouseScroll(int scroll_amount);

    float x() const;
    float y() const;
    float width() const;
    float height() const;

  private:
    KakouneClient **m_focused_client;
    PaneLayout *m_pane_layout;
    Window *m_window;
    domain::FontManager *m_font_manager;
    InfoBoxView *m_info_box_view;

    std::function<void()> m_mark_dirty;
};

#endif
