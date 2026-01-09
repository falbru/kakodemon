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
    void update();

  private:
    std::string keyEventToKakouneKey(const domain::KeyEvent &event);

    KakouneClient *m_kakoune_client;
    std::vector<std::string> m_pending_keys;
};

#endif
