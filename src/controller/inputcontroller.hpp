#ifndef INPUTCONTROLLER_HPP_INCLUDED
#define INPUTCONTROLLER_HPP_INCLUDED

#include "model/kakouneclient.hpp"
#include <memory>

class InputController
{
  public:
    InputController();
    ~InputController();

    void init(std::shared_ptr<KakouneClient> kakoune_client);
    void onCharacterInsert(std::string character);

  private:
    std::shared_ptr<KakouneClient> m_kakoune_client;
};

#endif
