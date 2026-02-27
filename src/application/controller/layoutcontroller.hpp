#ifndef LAYOUTCONTROLLER_HPP_INCLUDED
#define LAYOUTCONTROLLER_HPP_INCLUDED

#include "application/model/panelayout.hpp"

class Window;

class LayoutController
{
  public:
    LayoutController();

    void init(PaneLayout *pane_layout, ClientManager *client_manager, Window *window);

  private:
    PaneLayout *m_pane_layout;
};

#endif
