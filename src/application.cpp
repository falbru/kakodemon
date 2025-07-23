#include "application.hpp"
#include "input/keys.hpp"
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

    glfwSetCharModsCallback(m_window, [](GLFWwindow* window, unsigned int codepoint, int mods) {
        Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        app->onCharacterInsert(codepoint, mods);
    });

    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        app->onKeyInput(key, scancode, action, mods);
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

void Application::onKeyInput(int key, int scancode, int action, int mods)
{
    if (action != GLFW_PRESS) {
        return;
    }

    std::optional<KeyEvent> event = glfwSpecialKeyToKeyEvent(key, mods);

    if (event.has_value()) {
        m_input_controller->onKeyInput(event.value());
    }
}

void Application::onCharacterInsert(unsigned int codepoint, int mods)
{
    m_input_controller->onKeyInput(glfwCharToKeyEvent(codepoint, mods));
}

std::optional<KeyEvent> Application::glfwSpecialKeyToKeyEvent(int key, int mods) {
    static const std::unordered_map<int, SpecialKey> glfw_special_keys = {
        {GLFW_KEY_ESCAPE, ESCAPE},
        {GLFW_KEY_TAB, TAB},
        {GLFW_KEY_ENTER, RETURN},
        {GLFW_KEY_BACKSPACE, BACKSPACE},
        {GLFW_KEY_DELETE, DELETE},
        {GLFW_KEY_LEFT, LEFT},
        {GLFW_KEY_RIGHT, RIGHT},
        {GLFW_KEY_UP, UP},
        {GLFW_KEY_DOWN, DOWN},
        {GLFW_KEY_HOME, HOME},
        {GLFW_KEY_END, END},
        {GLFW_KEY_PAGE_UP, PAGE_UP},
        {GLFW_KEY_PAGE_DOWN, PAGE_DOWN},
        {GLFW_KEY_INSERT, INSERT}
    };

    auto it = glfw_special_keys.find(key);
    if (it == glfw_special_keys.end()) {
        return std::nullopt;
    }

    KeyEvent event;
    event.key = it->second;

    event.modifiers = 0;
    if (mods & GLFW_MOD_CONTROL) {
        event.modifiers |= CONTROL;
    }
    if (mods & GLFW_MOD_SHIFT) {
        event.modifiers |= SHIFT;
    }
    if (mods & GLFW_MOD_ALT) {
        event.modifiers |= ALT;
    }

    return event;
}

std::string utf8_encode(unsigned int utf)
{
    if (utf <= 0x7F) {
        // Plain ASCII
        return std::string(1, (char) utf);
    }
    else if (utf <= 0x07FF) {
        // 2-byte unicode
        std::string result;
        result += (char) (((utf >> 6) & 0x1F) | 0xC0);
        result += (char) (((utf >> 0) & 0x3F) | 0x80);
        return result;
    }
    else if (utf <= 0xFFFF) {
        // 3-byte unicode
        std::string result;
        result += (char) (((utf >> 12) & 0x0F) | 0xE0);
        result += (char) (((utf >> 6) & 0x3F) | 0x80);
        result += (char) (((utf >> 0) & 0x3F) | 0x80);
        return result;
    }
    else if (utf <= 0x10FFFF) {
        // 4-byte unicode
        std::string result;
        result += (char) (((utf >> 18) & 0x07) | 0xF0);
        result += (char) (((utf >> 12) & 0x3F) | 0x80);
        result += (char) (((utf >> 6) & 0x3F) | 0x80);
        result += (char) (((utf >> 0) & 0x3F) | 0x80);
        return result;
    }
    else {
        // error - use replacement character
        std::string result;
        result += (char) 0xEF;
        result += (char) 0xBF;
        result += (char) 0xBD;
        return result;
    }
}

KeyEvent Application::glfwCharToKeyEvent(unsigned int codepoint, int mods) {
    KeyEvent event;

    event.modifiers = 0;
    if (mods & GLFW_MOD_CONTROL) {
        event.modifiers |= CONTROL;
    }
    if (mods & GLFW_MOD_SHIFT) {
        event.modifiers |= SHIFT;
    }
    if (mods & GLFW_MOD_ALT) {
        event.modifiers |= ALT;
    }

    event.key = utf8_encode(codepoint);

    return event;
}
