#ifndef LAYOUTCONTROLLER_HPP_INCLUDED
#define LAYOUTCONTROLLER_HPP_INCLUDED

#include "application/model/panelayout.hpp"
class LayoutController
{
  public:
    LayoutController();

    void init(PaneLayout *pane_layout, ClientManager *client_manager);

    void onWindowResize(int width, int height);

  private:
    PaneLayout *m_pane_layout;
};

#endif
