#ifndef MODEL_PANELAYOUT_HPP_INCLUDED
#define MODEL_PANELAYOUT_HPP_INCLUDED

#include "application/model/kakouneclient.hpp"
#include "domain/geometry.hpp"
#include <memory>
#include <vector>

struct Pane
{
    KakouneClient *client;
    domain::Rectangle bounds;
};

class PaneLayout
{
  public:
    void init(std::vector<std::unique_ptr<KakouneClient>> *clients);
    void arrange(float width, float height);
    Pane *findPaneAt(float x, float y);
    Pane *findPaneForClient(KakouneClient *client);
    const std::vector<Pane> &getPanes() const;

    void addArrangeCallback(std::function<void(const std::vector<Pane> &)> callback);
    void notifyArrangeCallbacks() const;

  private:
    std::vector<Pane> m_panes;
    std::vector<std::unique_ptr<KakouneClient>> *m_clients;

    std::vector<std::function<void(const std::vector<Pane> &)>> m_callbacks;
};

#endif
