#ifndef EDITORCONTROLLER_HPP_INCLUDED
#define EDITORCONTROLLER_HPP_INCLUDED

#include <memory>

#include "model/kakouneclient.hpp"
#include "view/kakounecontentview.hpp"

class EditorController
{
  public:
    EditorController();
    ~EditorController();

    void init(std::shared_ptr<KakouneClient> kakoune_client, std::shared_ptr<KakouneContentView> kakoune_content_view);
    void update();

  private:
    std::shared_ptr<KakouneClient> m_kakoune_client;
    std::shared_ptr<KakouneContentView> m_kakoune_content_view;
};

#endif
