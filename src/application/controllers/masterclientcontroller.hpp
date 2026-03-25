#ifndef MASTERCLIENTCONTROLLER_HPP_INCLUDED
#define MASTERCLIENTCONTROLLER_HPP_INCLUDED

#include "application/model/clientmanager.hpp"
#include "application/model/panelayout.hpp"
#include "domain/ports/kakounesession.hpp"
#include <string>
#include <vector>

class MasterClientController
{
  public:
    void init(domain::KakouneSession *session, PaneLayout *pane_layout);
    ~MasterClientController();

    void updateMasterClients();

  private:
    domain::KakouneSession *m_session = nullptr;
    PaneLayout *m_pane_layout = nullptr;
    ObserverId m_arrange_observer;
    std::vector<std::string> m_cached_master_names;

    std::vector<std::string> getMasterClientNames() const;
};

#endif
