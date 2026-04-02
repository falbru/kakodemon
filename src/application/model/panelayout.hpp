#ifndef MODEL_PANELAYOUT_HPP_INCLUDED
#define MODEL_PANELAYOUT_HPP_INCLUDED

#include "application/model/clientmanager.hpp"
#include "application/model/focusedclientstack.hpp"
#include "application/model/kakouneclient.hpp"
#include "domain/geometry.hpp"
#include "domain/observerlist.hpp"
#include <vector>

enum class LayoutType
{
    TALL,
    WIDE,
    FULL,
};

struct Pane
{
    KakouneClient *client;
    domain::Rectangle bounds;
};

class PaneLayout
{
  public:
    static constexpr float BORDER_WIDTH = 1.0f;

    void init(ClientManager *client_manager, FocusedClientStack *focused_client_stack);
    void arrange();
    void setBounds(const domain::Rectangle &bounds);
    void setLayoutType(LayoutType layout_type);
    LayoutType getPreviousLayoutType();
    LayoutType getLayoutType() const;
    void setNumMasters(int num_masters);
    int getNumMasters() const;
    void setMasterRatio(float master_ratio);
    float getMasterRatio() const;

    Pane *findPaneAt(float x, float y);
    Pane *findPaneForClient(KakouneClient *client);
    const std::vector<Pane> &getPanes() const;

    domain::ObserverId onArrange(std::function<void(const std::vector<Pane> &)> callback);
    domain::ObserverId onNumMastersChanged(std::function<void(int)> callback);
    void removeObserver(domain::ObserverId id);

  private:
    void arrangeTall();
    void arrangeWide();
    void arrangeFull();

    std::vector<Pane> m_panes;
    LayoutType m_layout_type = LayoutType::TALL;
    LayoutType m_previous_layout_type = LayoutType::TALL;
    int m_num_masters = 1;
    float m_master_ratio = 0.6f;

    FocusedClientStack *m_focused_client_stack;
    ClientManager *m_client_manager;
    domain::ObserverList<const std::vector<Pane> &> m_arrange_observers;
    domain::ObserverList<int> m_num_masters_changed_observers;
    domain::Rectangle m_bounds;
};

#endif
