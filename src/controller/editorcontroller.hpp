#ifndef EDITORCONTROLLER_HPP_INCLUDED
#define EDITORCONTROLLER_HPP_INCLUDED

#include <memory>

#include "kakoune/kakouneclientprocess.hpp"
#include "kakoune/kakouneframestatemanager.hpp"
#include "model/kakouneclient.hpp"
#include "view/kakounecontentview.hpp"

class EditorController
{
  public:
    EditorController();

    void init(std::shared_ptr<KakouneClient> kakoune_client, std::shared_ptr<KakouneClientProcess> kakoune_process,
              std::shared_ptr<KakouneContentView> kakoune_content_view);
    void update();

  private:
    std::shared_ptr<KakouneClient> m_kakoune_client;
    std::shared_ptr<KakouneContentView> m_kakoune_content_view;

    std::unique_ptr<KakouneFrameStateManager> m_frame_state_manager;
};

#endif
