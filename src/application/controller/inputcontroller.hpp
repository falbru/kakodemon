#ifndef INPUTCONTROLLER_HPP_INCLUDED
#define INPUTCONTROLLER_HPP_INCLUDED

#include "application/model/clientmanager.hpp"
#include "application/model/focusedclientstack.hpp"
#include "application/model/kakouneclient.hpp"
#include "domain/keys.hpp"
#include "domain/ports/window.hpp"

class InputController
{
  public:
    InputController();
    ~InputController();

    void init(FocusedClientStack *focused_client_stack, ClientManager *client_manager, domain::Window *window);
    void onKeyInput(const domain::KeyEvent &event);
    void update();

  private:
    std::string keyEventToKakouneKey(const domain::KeyEvent &event);
    bool tryHandleGlobalKeybinding(const domain::KeyEvent &event);
    void focusNext();
    void focusPrev();

    FocusedClientStack *m_focused_client_stack;
    ClientManager *m_client_manager;
    std::vector<std::string> m_pending_keys;
};

#endif
