#include "domain/ports/window.hpp"

void domain::Window::setNeedsRerender()
{
    m_needs_rerender = true;
}

bool domain::Window::needsRerender() const
{
    return m_needs_rerender;
}

void domain::Window::clearRerender()
{
    m_needs_rerender = false;
}
