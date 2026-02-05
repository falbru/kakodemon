#ifndef MODEL_PANELAYOUT_HPP_INCLUDED
#define MODEL_PANELAYOUT_HPP_INCLUDED

#include "application/model/clientmanager.hpp"
#include "application/model/kakouneclient.hpp"
#include "domain/geometry.hpp"
#include <map>
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
    void notifyArrangeObservers() const;

    std::vector<Pane> m_panes;
    ClientManager *m_client_manager;

    ObserverId m_next_observer_id = 0;
    std::map<ObserverId, std::function<void(const std::vector<Pane> &)>> m_arrange_observers;

    domain::Rectangle m_bounds;
};

#endif
