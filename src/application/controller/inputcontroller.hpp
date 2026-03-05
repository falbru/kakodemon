#ifndef INPUTCONTROLLER_HPP_INCLUDED
#define INPUTCONTROLLER_HPP_INCLUDED

#include "application/model/focusedclientstack.hpp"
#include "application/model/kakouneclient.hpp"
#include "domain/keys.hpp"
#include "domain/ports/window.hpp"

class InputController
{
  public:
    InputController();
    ~InputController();

    void init(FocusedClientStack *focused_client_stack, domain::Window *window);
    void onKeyInput(const domain::KeyEvent &event);
    void update();

  private:
    std::string keyEventToKakouneKey(const domain::KeyEvent &event);

    FocusedClientStack *m_focused_client_stack;
    std::vector<std::string> m_pending_keys;
};

#endif
