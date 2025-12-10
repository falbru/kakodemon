#ifndef INPUTCONTROLLER_HPP_INCLUDED
#define INPUTCONTROLLER_HPP_INCLUDED

#include "core/keys.hpp"
#include "kakoune/kakouneclientprocess.hpp"
#include "model/kakouneclient.hpp"

class InputController
{
  public:
    InputController();
    ~InputController();

    void init(KakouneClient *kakoune_client, KakouneClientProcess *kakoune_process);
    void onKeyInput(const core::KeyEvent &event);

  private:
    std::string keyEventToKakouneKey(const core::KeyEvent &event);

    KakouneClient *m_kakoune_client;
    KakouneClientProcess *m_kakoune_process;
};

#endif
