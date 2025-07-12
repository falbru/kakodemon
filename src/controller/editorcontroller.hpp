#ifndef EDITORCONTROLLER_HPP_INCLUDED
#define EDITORCONTROLLER_HPP_INCLUDED

#include <memory>

#include "../model/kakouneclientprocess.hpp"

class EditorController
{
  public:
    EditorController();
    ~EditorController();

    void init();
    void update();

  private:
    std::unique_ptr<KakouneClientProcess> m_kak_proc;
};

#endif
