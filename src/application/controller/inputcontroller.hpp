#ifndef INPUTCONTROLLER_HPP_INCLUDED
#define INPUTCONTROLLER_HPP_INCLUDED

#include "application/model/clientmanager.hpp"
#include "application/model/focusedclientstack.hpp"
#include "application/model/kakouneclient.hpp"
#include "application/model/panelayout.hpp"
#include "domain/keys.hpp"
#include "domain/ports/window.hpp"

class InputController
{
  public:
    InputController();
    ~InputController();

    void init(FocusedClientStack *focused_client_stack, ClientManager *client_manager, domain::Window *window,
              PaneLayout *pane_layout);
    void onKeyInput(const domain::KeyEvent &event);
    void update();

  private:
    std::string keyEventToKakouneKey(const domain::KeyEvent &event);
    bool tryHandleGlobalKeybinding(const domain::KeyEvent &event);
    void focusNext();
    void focusPrev();

    FocusedClientStack *m_focused_client_stack;
    ClientManager *m_client_manager;
    PaneLayout *m_pane_layout;
    domain::Window *m_window;
    std::vector<std::string> m_pending_keys;
};

#endif
