#ifndef EDITORCONTROLLER_HPP_INCLUDED
#define EDITORCONTROLLER_HPP_INCLUDED

#include <memory>

#include "../model/kakouneclientprocess.hpp"
#include "../view/triangleview.hpp"

class EditorController
{
  public:
    EditorController();
    ~EditorController();

    void init(std::shared_ptr<TriangleView> triangle_view);
    void update();

  private:
    std::shared_ptr<TriangleView> m_triangle_view;

    std::unique_ptr<KakouneClientProcess> m_kak_proc;
};

#endif
