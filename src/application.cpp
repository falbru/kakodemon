#include "application.hpp"
#include "opengl.hpp"
#include "controller/editorcontroller.hpp"
#include "controller/inputcontroller.hpp"
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

    glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int codepoint) {
        Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        app->onCharacterInsert(codepoint);
    });
}

void Application::initMVC() {
    int width, height;
    glfwGetWindowSize(m_window, &width, &height);

    m_editor_controller = std::make_shared<EditorController>();
    m_input_controller = std::make_shared<InputController>();
    m_kakoune_client = std::make_shared<KakouneClient>();
    m_kakoune_process = std::make_shared<KakouneClientProcess>("default");
    m_kakoune_content_view = std::make_shared<KakouneContentView>();

    m_input_controller->init(m_kakoune_client, m_kakoune_process);
    m_kakoune_content_view->init(width, height);
    m_kakoune_process->start();
    m_editor_controller->init(m_kakoune_client, m_kakoune_process, m_kakoune_content_view);
    m_editor_controller->onWindowResize(width, height);
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
    m_editor_controller->onWindowResize(width, height);
}

std::string utf32_to_utf8(unsigned int codepoint) {  // TODO fix
    std::string result;

    if (codepoint <= 0x7F) {
        // 1-byte sequence
        result += static_cast<char>(codepoint);
    } else if (codepoint <= 0x7FF) {
        // 2-byte sequence
        result += static_cast<char>(0xC0 | (codepoint >> 6));
        result += static_cast<char>(0x80 | (codepoint & 0x3F));
    } else if (codepoint <= 0xFFFF) {
        // 3-byte sequence
        result += static_cast<char>(0xE0 | (codepoint >> 12));
        result += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
        result += static_cast<char>(0x80 | (codepoint & 0x3F));
    } else if (codepoint <= 0x10FFFF) {
        // 4-byte sequence
        result += static_cast<char>(0xF0 | (codepoint >> 18));
        result += static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F));
        result += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
        result += static_cast<char>(0x80 | (codepoint & 0x3F));
    }

    return result;
}

void Application::onCharacterInsert(unsigned int codepoint)
{
    m_input_controller->onCharacterInsert(utf32_to_utf8(codepoint));
}
