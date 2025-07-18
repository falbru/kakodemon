#include "application.hpp"
#include "controller/editorcontroller.hpp"
#include "model/kakouneclient.hpp"
#include "view/kakounecontentview.hpp"
#include <memory>

Application::Application()
{

}

Application::~Application()
{
    glfwTerminate();
}

void Application::init()
{
    initGLFW();
    initMVC();
}

void Application::initGLFW()
{
    if (!glfwInit()) {
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, "Hello World", NULL, NULL);
    if (!m_window)
    {
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_window);
    glfwSetWindowUserPointer(m_window, this);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return;
    }

    glViewport(0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);

    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
        Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        app->onWindowResize(width, height);
    });
}

void Application::initMVC() {
    int width, height;
    glfwGetWindowSize(m_window, &width, &height);

    m_editor_controller = std::make_shared<EditorController>();
    m_kakoune_client = std::make_shared<KakouneClient>("editor");
    m_kakoune_content_view = std::make_shared<KakouneContentView>();

    m_editor_controller->init(m_kakoune_client, m_kakoune_content_view);
    m_kakoune_content_view->init(width, height);
    m_kakoune_client->init();
}

void Application::run()
{
    while (!glfwWindowShouldClose(m_window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        m_editor_controller->update();

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

void Application::onWindowResize(int width, int height)
{
    glViewport(0, 0, width, height);
    m_kakoune_content_view->onWindowResize(width, height);
}
