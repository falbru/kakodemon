#include "application.hpp"
#include "GLFW/glfw3.h"
#include "adapters/opengl/font.hpp"
#include "adapters/opengl/renderer.hpp"
#include "domain/fontmanager.hpp"
#include "domain/mouse.hpp"
#include <chrono>
#include <thread>

opengl::GLFWApplication::GLFWApplication() {
}

opengl::GLFWApplication::~GLFWApplication() {
    m_font_manager.reset();
    m_renderer.reset();

    glfwDestroyCursor(m_cursor_ibeam);
    glfwDestroyCursor(m_cursor_pointer);

    glfwTerminate();
}

void opengl::GLFWApplication::init(const CliConfig& config) {
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

    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
        GLFWApplication* app = static_cast<GLFWApplication*>(glfwGetWindowUserPointer(window));
        app->onGLFWMouseMove(xpos, ypos);
    });

    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
        GLFWApplication* app = static_cast<GLFWApplication*>(glfwGetWindowUserPointer(window));
        app->onGLFWMouseButton(button, action, mods);
    });

    glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset) {
        GLFWApplication* app = static_cast<GLFWApplication*>(glfwGetWindowUserPointer(window));
        app->onMouseScroll(yoffset);
    });

    int framebuffer_width, framebuffer_height;
    glfwGetFramebufferSize(m_window, &framebuffer_width, &framebuffer_height);

    m_renderer = std::make_unique<opengl::Renderer>();

    m_cursor_ibeam = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
    m_cursor_pointer = glfwCreateStandardCursor(GLFW_POINTING_HAND_CURSOR);

    auto font_factory = [](domain::FontEngine *engine, domain::FontManager *font_manager) -> std::unique_ptr<domain::Font> {
        return std::make_unique<opengl::Font>(engine);
    };

    m_font_manager = std::make_unique<domain::FontManager>(
        std::move(m_font_resolver),
        m_font_engine_factory,
        font_factory
    );

    m_renderer->init(framebuffer_width, framebuffer_height, m_font_manager.get());

    Application::init(config);

    m_ui_options->font = m_font_manager->getFontFromName("Ubuntu Mono:size=14");
    onWindowResize(framebuffer_width, framebuffer_height);
}

void opengl::GLFWApplication::run() {
    while (!glfwWindowShouldClose(m_window))
    {
        auto frame_start = std::chrono::high_resolution_clock::now();

        glfwPollEvents();

        updateControllers();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(m_clear_color.r, m_clear_color.g, m_clear_color.b, 1.0f);

        renderControllers();

        glfwSwapBuffers(m_window);

        auto frame_end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = frame_end - frame_start;

        double remaining_time = TARGET_FRAME_TIME_MS - elapsed.count();
        if (remaining_time > 0) {
            std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(remaining_time));
        }
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

    event.modifiers = toDomainModifiers(mods);

    return event;
}

void opengl::GLFWApplication::onGLFWMouseMove(float cursor_x, float cursor_y) {
    m_cursor_x = cursor_x;
    m_cursor_y = cursor_y;

    this->onMouseMove(cursor_x, cursor_y);
}

void opengl::GLFWApplication::onGLFWMouseButton(int button, int action, int mods) {
    domain::MouseButtonEvent event;

    switch(button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            event.button = domain::MouseButton::LEFT;
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            event.button = domain::MouseButton::MIDDLE;
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            event.button = domain::MouseButton::RIGHT;
            break;
    }

    switch(action) {
        case GLFW_PRESS:
            event.action = domain::MouseButtonAction::PRESS;
            break;
        case GLFW_RELEASE:
            event.action = domain::MouseButtonAction::RELEASE;
            break;
    }

    event.modifiers = toDomainModifiers(mods);
    event.x = m_cursor_x;
    event.y = m_cursor_y;

    onMouseButton(event);
}

int opengl::GLFWApplication::toDomainModifiers(int mods) {
    int domain_modifiers = 0;

    if (mods & GLFW_MOD_CONTROL) {
        domain_modifiers |= domain::CONTROL;
    }
    if (mods & GLFW_MOD_SHIFT) {
        domain_modifiers |= domain::SHIFT;
    }
    if (mods & GLFW_MOD_ALT) {
        domain_modifiers |= domain::ALT;
    }

    return domain_modifiers;
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

void opengl::GLFWApplication::setCursor(domain::Cursor cursor) {
    Application::setCursor(cursor);

    switch(cursor) {
        case domain::Cursor::DEFAULT:
            glfwSetCursor(m_window, NULL);
            break;
        case domain::Cursor::IBEAM:
            glfwSetCursor(m_window, m_cursor_ibeam);
            break;
        case domain::Cursor::POINTER:
            glfwSetCursor(m_window, m_cursor_pointer);
            break;
    }
}
