#ifndef INPUTCONTROLLER_HPP_INCLUDED
#define INPUTCONTROLLER_HPP_INCLUDED

#include "core/keys.hpp"
#include "kakoune/kakouneclientprocess.hpp"
#include "model/kakouneclient.hpp"
#include <memory>

class InputController
{
  public:
    InputController();
    ~InputController();

    void init(std::shared_ptr<KakouneClient> kakoune_client, std::shared_ptr<KakouneClientProcess> kakoune_process);
    void onKeyInput(const KeyEvent &event);

  private:
    std::string keyEventToKakouneKey(const KeyEvent &event);

    std::shared_ptr<KakouneClient> m_kakoune_client;
    std::shared_ptr<KakouneClientProcess> m_kakoune_process;
};

#endif
