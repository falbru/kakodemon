#ifndef MODEL_FOCUSEDCLIENTSTACK_HPP_INCLUDED
#define MODEL_FOCUSEDCLIENTSTACK_HPP_INCLUDED

#include "application/model/kakouneclient.hpp"
#include "domain/observerlist.hpp"

#include <functional>
#include <vector>

class FocusedClientStack
{
  public:
    KakouneClient *focused() const;
    void focus(KakouneClient *client);
    void remove(KakouneClient *client);

    domain::ObserverId onFocusChanged(std::function<void(KakouneClient *previous, KakouneClient *next)> callback);
    void removeObserver(domain::ObserverId id);

  private:
    std::vector<KakouneClient *> m_stack;
    domain::ObserverList<KakouneClient *, KakouneClient *> m_focus_changed_observers;
};

#endif
