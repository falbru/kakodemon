#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>

#include "controller/editorcontroller.hpp"
#include "view/triangleview.hpp"

int main(void)
{
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Init Glad */
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    std::shared_ptr<EditorController> editor_controller = std::make_shared<EditorController>();

    std::shared_ptr<TriangleView> triangle_view = std::make_shared<TriangleView>();
    triangle_view->init();

    editor_controller->init(triangle_view);

    double lastTime = glfwGetTime();
    int frameCount = 0;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime();
        frameCount++;

        // Print FPS every second
        if (currentTime - lastTime >= 1.0) {
            std::cout << "FPS: " << frameCount << std::endl;
            frameCount = 0;
            lastTime = currentTime;
        }

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        editor_controller->update();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
