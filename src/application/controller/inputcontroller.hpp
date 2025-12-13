#ifndef INPUTCONTROLLER_HPP_INCLUDED
#define INPUTCONTROLLER_HPP_INCLUDED

#include "application/model/kakouneclient.hpp"
#include "domain/keys.hpp"
#include "kakoune/kakouneclientprocess.hpp"

class InputController
{
  public:
    InputController();
    ~InputController();

    void init(KakouneClient *kakoune_client, KakouneClientProcess *kakoune_process);
    void onKeyInput(const domain::KeyEvent &event);

  private:
    std::string keyEventToKakouneKey(const domain::KeyEvent &event);

    KakouneClient *m_kakoune_client;
    KakouneClientProcess *m_kakoune_process;
};

#endif
