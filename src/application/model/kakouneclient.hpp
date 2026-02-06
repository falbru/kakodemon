#ifndef KAKOUNECLIENT_HPP_INCLUDED
#define KAKOUNECLIENT_HPP_INCLUDED

#include "application/model/viewstate.hpp"
#include "application/observerid.hpp"
#include "domain/ports/kakouneinterface.hpp"
#include "domain/ports/kakounesession.hpp"
#include "domain/uioptions.hpp"
#include <functional>
#include <map>
#include <memory>

class KakouneClient
{
  public:
    KakouneClient(domain::KakouneSession *session, std::unique_ptr<domain::KakouneInterface> interface);

    const domain::UIOptions &uiOptions() const;
    void setUIOptions(domain::UIOptions ui_options);

    ObserverId onUIOptionsChanged(std::function<void(const domain::UIOptions &)> callback);
    void removeUIOptionsObserver(ObserverId id);

    domain::KakouneState state;
    domain::KakouneSession *session;
    std::unique_ptr<domain::KakouneInterface> interface;

    MenuViewState prompt_menu_state;
    MenuViewState inline_menu_state;
    MenuViewState search_menu_state;
    InputViewState status_line_state;
    InfoBoxViewState info_box_state;

  private:
    void notifyUIOptionsChangedObservers();

    domain::UIOptions m_ui_options;

    ObserverId m_next_observer_id = 0;
    std::map<ObserverId, std::function<void(const domain::UIOptions &)>> m_ui_options_observers;
};

#endif
