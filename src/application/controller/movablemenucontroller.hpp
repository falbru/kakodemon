#ifndef CONTROLLER_MOVABLEMENUCONTROLLER_HPP_INCLUDED
#define CONTROLLER_MOVABLEMENUCONTROLLER_HPP_INCLUDED

#include "application/view/multistyledmenu.hpp"
#include "domain/eventfilters.hpp"
#include "domain/observerlist.hpp"
#include "domain/ports/window.hpp"

class MovableMenuController
{
  public:
    MovableMenuController();
    ~MovableMenuController();

    void init(MultiStyledMenuView *menu, domain::Window *window);

  private:
    MultiStyledMenuView *m_menu;
    domain::Window *m_window;

    bool m_is_dragging;
    float m_last_drag_cursor_x, m_last_drag_cursor_y;

    domain::EventFilterId m_event_filter_id;
    domain::ObserverId m_visiblity_changed_observer_id;
};

#endif
