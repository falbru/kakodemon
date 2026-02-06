#include "window.hpp"
#include "GLFW/glfw3.h"
#include "adapters/opengl/font.hpp"
#include "adapters/opengl/renderer.hpp"
#include "domain/fontmanager.hpp"
#include "domain/mouse.hpp"

opengl::GLFWWindow::GLFWWindow() {
}

opengl::GLFWWindow::~GLFWWindow() {
    m_font_manager.reset();
    m_renderer.reset();

    if (m_cursor_ibeam) glfwDestroyCursor(m_cursor_ibeam);
    if (m_cursor_pointer) glfwDestroyCursor(m_cursor_pointer);

    if (m_window) glfwTerminate();
}

void opengl::GLFWWindow::setFontDependencies(
    std::unique_ptr<domain::FontResolver> resolver,
    FontEngineFactory engine_factory)
{
    m_font_resolver = std::move(resolver);
    m_font_engine_factory = engine_factory;
}

void opengl::GLFWWindow::init(bool maximized) {
    if (!glfwInit()) {
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    if (maximized) {
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
    }

    glfwWindowHintString(GLFW_WAYLAND_APP_ID, "kakodemon");
    glfwWindowHintString(GLFW_X11_CLASS_NAME, "Kakodemon");
    glfwWindowHintString(GLFW_X11_INSTANCE_NAME, "kakodemon");

    m_window = glfwCreateWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, "Kakodemon", NULL, NULL);
    if (!m_window)
    {
        glfwTerminate();
        return;
    }

    glfwSetWindowSizeLimits(m_window, 636, 424, GLFW_DONT_CARE, GLFW_DONT_CARE);

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(0);
    glfwSetWindowUserPointer(m_window, this);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return;
    }

    int framebuffer_width, framebuffer_height;
    glfwGetFramebufferSize(m_window, &framebuffer_width, &framebuffer_height);
    glViewport(0, 0, framebuffer_width, framebuffer_height);

    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
        GLFWWindow* self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
        self->m_renderer->onWindowResize(width, height);
        for (auto& [id, cb] : self->m_resize_observers) {
            cb(width, height);
        }
    });

    glfwSetCharModsCallback(m_window, [](GLFWwindow* window, unsigned int codepoint, int mods) {
        GLFWWindow* self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
        self->onGLFWCharacterInsert(codepoint, mods);
    });

    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        GLFWWindow* self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
        self->onGLFWKeyInput(key, scancode, action, mods);
    });

    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
        GLFWWindow* self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
        self->onGLFWMouseMove(xpos, ypos);
    });

    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
        GLFWWindow* self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
        self->onGLFWMouseButton(button, action, mods);
    });

    glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset) {
        GLFWWindow* self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
        for (auto& [id, cb] : self->m_mouse_scroll_observers) {
            cb(yoffset);
        }
    });

    glfwSetWindowMaximizeCallback(m_window, [](GLFWwindow* window, int maximized) {
        GLFWWindow* self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
        for (auto& [id, cb] : self->m_maximized_changed_observers) {
            cb(maximized == GLFW_TRUE);
        }
    });

    glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
        GLFWWindow* self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
        for (auto& [id, cb] : self->m_close_observers) {
            cb();
        }
    });

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

    m_renderer->init(framebuffer_width, framebuffer_height);

    glfwSwapBuffers(m_window);
}

void opengl::GLFWWindow::waitEvents() {
    glfwWaitEvents();
}

void opengl::GLFWWindow::wakeEventLoop() {
    glfwPostEmptyEvent();
}

void opengl::GLFWWindow::renderBegin() {
    glClearColor(m_clear_color.r, m_clear_color.g, m_clear_color.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void opengl::GLFWWindow::renderEnd() {
    glfwSwapBuffers(m_window);
}

void opengl::GLFWWindow::setClearColor(domain::RGBAColor color) {
    m_clear_color = color;
}

void opengl::GLFWWindow::setTitle(const std::string& title) {
    glfwSetWindowTitle(m_window, title.c_str());
}

void opengl::GLFWWindow::setCursor(domain::Cursor cursor) {
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

float opengl::GLFWWindow::getWidth() {
    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);
    return width;
}

float opengl::GLFWWindow::getHeight() {
    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);
    return height;
}

domain::Renderer *opengl::GLFWWindow::getRenderer() {
    return m_renderer.get();
}

domain::FontManager *opengl::GLFWWindow::getFontManager() {
    return m_font_manager.get();
}

void opengl::GLFWWindow::onGLFWKeyInput(int key, int scancode, int action, int mods)
{
    if (action != GLFW_PRESS && action != GLFW_REPEAT) {
        return;
    }

    std::optional<domain::KeyEvent> event = glfwSpecialKeyToKeyEvent(key, mods);

    if (event.has_value()) {
        for (auto& [id, cb] : m_key_input_observers) {
            cb(event.value());
        }
    }
}

void opengl::GLFWWindow::onGLFWCharacterInsert(unsigned int codepoint, int mods)
{
    domain::KeyEvent event = glfwCharToKeyEvent(codepoint, mods);
    for (auto& [id, cb] : m_key_input_observers) {
        cb(event);
    }
}

std::optional<domain::KeyEvent> opengl::GLFWWindow::glfwSpecialKeyToKeyEvent(int key, int mods) {
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

void opengl::GLFWWindow::onGLFWMouseMove(float cursor_x, float cursor_y) {
    m_cursor_x = cursor_x;
    m_cursor_y = cursor_y;

    for (auto& [id, cb] : m_mouse_move_observers) {
        cb(cursor_x, cursor_y);
    }
}

void opengl::GLFWWindow::onGLFWMouseButton(int button, int action, int mods) {
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

    for (auto& [id, cb] : m_mouse_button_observers) {
        cb(event);
    }
}

int opengl::GLFWWindow::toDomainModifiers(int mods) {
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

domain::KeyEvent opengl::GLFWWindow::glfwCharToKeyEvent(unsigned int codepoint, int mods) {
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

ObserverId opengl::GLFWWindow::onResize(std::function<void(int, int)> callback) {
    ObserverId id = m_next_observer_id++;
    m_resize_observers[id] = callback;
    return id;
}

ObserverId opengl::GLFWWindow::onKeyInput(std::function<void(domain::KeyEvent)> callback) {
    ObserverId id = m_next_observer_id++;
    m_key_input_observers[id] = callback;
    return id;
}

ObserverId opengl::GLFWWindow::onMouseMove(std::function<void(float, float)> callback) {
    ObserverId id = m_next_observer_id++;
    m_mouse_move_observers[id] = callback;
    return id;
}

ObserverId opengl::GLFWWindow::onMouseButton(std::function<void(domain::MouseButtonEvent)> callback) {
    ObserverId id = m_next_observer_id++;
    m_mouse_button_observers[id] = callback;
    return id;
}

ObserverId opengl::GLFWWindow::onMouseScroll(std::function<void(double)> callback) {
    ObserverId id = m_next_observer_id++;
    m_mouse_scroll_observers[id] = callback;
    return id;
}

ObserverId opengl::GLFWWindow::onClose(std::function<void()> callback) {
    ObserverId id = m_next_observer_id++;
    m_close_observers[id] = callback;
    return id;
}

ObserverId opengl::GLFWWindow::onMaximizedChanged(std::function<void(bool)> callback) {
    ObserverId id = m_next_observer_id++;
    m_maximized_changed_observers[id] = callback;
    return id;
}

void opengl::GLFWWindow::removeObserver(ObserverId id) {
    m_resize_observers.erase(id);
    m_key_input_observers.erase(id);
    m_mouse_move_observers.erase(id);
    m_mouse_button_observers.erase(id);
    m_mouse_scroll_observers.erase(id);
    m_close_observers.erase(id);
    m_maximized_changed_observers.erase(id);
}
