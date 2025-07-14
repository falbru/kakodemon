#ifndef TRIANGLEVIEW_HPP_INCLUDED
#define TRIANGLEVIEW_HPP_INCLUDED

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class TriangleView {
public:
    TriangleView();

    void init();
    void render();
private:
    unsigned int m_shader_program;
    unsigned int m_vao;
};

#endif
