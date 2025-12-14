#ifndef INPUTCONTROLLER_HPP_INCLUDED
#define INPUTCONTROLLER_HPP_INCLUDED

#include "application/model/kakouneclient.hpp"
#include "domain/keys.hpp"

class InputController
{
  public:
    InputController();
    ~InputController();

    void init(KakouneClient *kakoune_client);
    void onKeyInput(const domain::KeyEvent &event);

  private:
    std::string keyEventToKakouneKey(const domain::KeyEvent &event);

    KakouneClient *m_kakoune_client;
};

#endif
