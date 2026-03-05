#ifndef MODEL_PANELAYOUT_HPP_INCLUDED
#define MODEL_PANELAYOUT_HPP_INCLUDED

#include "application/model/clientmanager.hpp"
#include "application/model/kakouneclient.hpp"
#include "domain/geometry.hpp"
#include "domain/observerlist.hpp"
#include <vector>

struct Pane
{
    KakouneClient *client;
    domain::Rectangle bounds;
};

class PaneLayout
{
  public:
    void init(ClientManager *client_manager);
    void arrange();
    void setBounds(const domain::Rectangle &bounds);

    Pane *findPaneAt(float x, float y);
    Pane *findPaneForClient(KakouneClient *client);
    const std::vector<Pane> &getPanes() const;

    ObserverId onArrange(std::function<void(const std::vector<Pane> &)> callback);
    void removeObserver(ObserverId id);

  private:
    std::vector<Pane> m_panes;
    ClientManager *m_client_manager;
    ObserverList<const std::vector<Pane> &> m_arrange_observers;
    domain::Rectangle m_bounds;
};

#endif
