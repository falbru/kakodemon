#include "application.hpp"
#include "adapters/freetype/freetypefontengine.hpp"
#include "font.hpp"
#include "renderer.hpp"
#include <memory>

opengl::GLFWApplication::GLFWApplication() {

}

opengl::GLFWApplication::~GLFWApplication() {
    glfwTerminate();
}

void opengl::GLFWApplication::init() {
    if (!glfwInit()) {
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, "Kakodemon", NULL, NULL);
    if (!m_window)
    {
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(0);
    glfwSetWindowUserPointer(m_window, this);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return;
    }

    glViewport(0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);

    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
        GLFWApplication* app = static_cast<GLFWApplication*>(glfwGetWindowUserPointer(window));
        app->onWindowResize(width, height);
    });

    glfwSetCharModsCallback(m_window, [](GLFWwindow* window, unsigned int codepoint, int mods) {
        GLFWApplication* app = static_cast<GLFWApplication*>(glfwGetWindowUserPointer(window));
        app->onGLFWCharacterInsert(codepoint, mods);
    });

    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        GLFWApplication* app = static_cast<GLFWApplication*>(glfwGetWindowUserPointer(window));
        app->onGLFWKeyInput(key, scancode, action, mods);
    });

    int framebuffer_width, framebuffer_height;
    glfwGetFramebufferSize(m_window, &framebuffer_width, &framebuffer_height);

    m_renderer = std::make_unique<opengl::Renderer>();
    m_renderer->init(framebuffer_width, framebuffer_height);

    Application::init();

    m_ui_options->font = std::make_unique<opengl::Font>(new FreeTypeFontEngine("/usr/share/fonts/truetype/ubuntu/UbuntuMono-R.ttf", 14)); // TODO TMP.
    onWindowResize(framebuffer_width, framebuffer_height); // TODO ensure it is called after controller initialization
}

void opengl::GLFWApplication::run() {
    while (!glfwWindowShouldClose(m_window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(m_clear_color.r, m_clear_color.g, m_clear_color.b, 1.0f);

        updateControllers();

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

void opengl::GLFWApplication::onGLFWKeyInput(int key, int scancode, int action, int mods)
{
    if (action != GLFW_PRESS && action != GLFW_REPEAT) {
        return;
    }

    std::optional<domain::KeyEvent> event = glfwSpecialKeyToKeyEvent(key, mods);

    if (event.has_value()) {
        onKeyInput(event.value());
    }
}

void opengl::GLFWApplication::onGLFWCharacterInsert(unsigned int codepoint, int mods)
{
    onKeyInput(glfwCharToKeyEvent(codepoint, mods));
}

std::optional<domain::KeyEvent> opengl::GLFWApplication::glfwSpecialKeyToKeyEvent(int key, int mods) {
    static const std::unordered_map<int, domain::SpecialKey> glfw_special_keys = {
        {GLFW_KEY_ESCAPE,    domain::SpecialKey::ESCAPE},
        {GLFW_KEY_TAB,       domain::SpecialKey::TAB},
        {GLFW_KEY_ENTER,     domain::SpecialKey::RETURN},
        {GLFW_KEY_BACKSPACE, domain::SpecialKey::BACKSPACE},
        {GLFW_KEY_DELETE,    domain::SpecialKey::DELETE},
        {GLFW_KEY_LEFT,      domain::SpecialKey::LEFT},
        {GLFW_KEY_RIGHT,     domain::SpecialKey::RIGHT},
        {GLFW_KEY_UP,        domain::SpecialKey::UP},
        {GLFW_KEY_DOWN,      domain::SpecialKey::DOWN},
        {GLFW_KEY_HOME,      domain::SpecialKey::HOME},
        {GLFW_KEY_END,       domain::SpecialKey::END},
        {GLFW_KEY_PAGE_UP,   domain::SpecialKey::PAGE_UP},
        {GLFW_KEY_PAGE_DOWN, domain::SpecialKey::PAGE_DOWN},
        {GLFW_KEY_INSERT,    domain::SpecialKey::INSERT}
    };

    auto it = glfw_special_keys.find(key);
    if (it == glfw_special_keys.end()) {
        return std::nullopt;
    }

    domain::KeyEvent event;
    event.key = it->second;

    event.modifiers = 0;
    if (mods & GLFW_MOD_CONTROL) {
        event.modifiers |= domain::CONTROL;
    }
    if (mods & GLFW_MOD_SHIFT) {
        event.modifiers |= domain::SHIFT;
    }
    if (mods & GLFW_MOD_ALT) {
        event.modifiers |= domain::ALT;
    }

    return event;
}

domain::KeyEvent opengl::GLFWApplication::glfwCharToKeyEvent(unsigned int codepoint, int mods) {
    domain::KeyEvent event;

    event.key = codepoint;

    event.modifiers = 0;
    if (mods & GLFW_MOD_CONTROL) {
        event.modifiers |= domain::CONTROL;
    }
    if (mods & GLFW_MOD_SHIFT) {
        event.modifiers |= domain::SHIFT;
    }
    if (mods & GLFW_MOD_ALT) {
        event.modifiers |= domain::ALT;
    }

    return event;
}
