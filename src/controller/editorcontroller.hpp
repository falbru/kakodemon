#ifndef EDITORCONTROLLER_HPP_INCLUDED
#define EDITORCONTROLLER_HPP_INCLUDED

#include <memory>

#include "kakoune/kakouneclientprocess.hpp"
#include "kakoune/kakouneframestatemanager.hpp"
#include "model/kakouneclient.hpp"
#include "view/kakounecontentview.hpp"
#include "view/statusbar.hpp"

class EditorController // TODO rename to KakouneContentViewController or something?
{
  public:
    EditorController();

    void init(std::shared_ptr<KakouneClient> kakoune_client, std::shared_ptr<KakouneClientProcess> kakoune_process,
              std::shared_ptr<KakouneContentView> kakoune_content_view, std::shared_ptr<StatusBarView> status_bar_view);
    void update();

    void onWindowResize(int width, int height);

    int width() const;
    int height() const;

  private:
    std::shared_ptr<KakouneClient> m_kakoune_client;
    std::shared_ptr<KakouneClientProcess> m_kakoune_process;
    std::shared_ptr<KakouneContentView> m_kakoune_content_view;
    std::shared_ptr<StatusBarView> m_status_bar_view;

    std::unique_ptr<KakouneFrameStateManager> m_frame_state_manager;

    int m_rows;
    int m_columns;
    int m_width;
    int m_height;
};

#endif
