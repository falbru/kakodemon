#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>

#include "controller/editorcontroller.hpp"
#include "model/kakouneclient.hpp"
#include "model/kakouneclientprocess.hpp"
#include "view/kakounecontentview.hpp"

std::shared_ptr<KakouneContentView> kakoune_content_view;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    kakoune_content_view->setSize(width, height);
}

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

    std::shared_ptr<KakouneClient> kakoune_client = std::make_shared<KakouneClient>();
    kakoune_client->process = std::make_unique<KakouneClientProcess>("editor");
    kakoune_client->process->start();

    kakoune_content_view = std::make_shared<KakouneContentView>();
    kakoune_content_view->init();
    kakoune_content_view->setSize(640, 480);

    editor_controller->init(kakoune_client, kakoune_content_view);

    double lastTime = glfwGetTime();
    int frameCount = 0;

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime();
        frameCount++;

        // Print FPS every second
        if (currentTime - lastTime >= 1.0)
        {
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
