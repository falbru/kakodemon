#include "editorcontroller.hpp"

#include <iostream>
#include <memory>

EditorController::EditorController()
{
    m_kak_proc = std::make_unique<KakouneClientProcess>("editor");
}

EditorController::~EditorController()
{
}

void EditorController::init(std::shared_ptr<TriangleView> triangle_view)
{
    m_triangle_view = triangle_view;
    m_kak_proc->start();
}

void EditorController::update()
{
    if (m_kak_proc->pollForRequest())
    {
        KakouneClientRequest request = m_kak_proc->getRequest();

        switch (request.type)
        {
        case KakouneRequestType::DRAW: {
            KakouneDrawRequestData &draw_data = std::get<KakouneDrawRequestData>(request.data);

            for (auto line : draw_data.lines)
            {
                for (auto atom : line.atoms)
                {
                    std::cout << atom.contents;
                }
            }
            break;
        }
        default:
            break;
        }
    }

    m_triangle_view->render();
}
