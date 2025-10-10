#include "layoutmanager.hpp"

Rect::Rect(float x, float y, float width, float height) : x(x), y(y), width(width), height(height) { }

LayoutManager::LayoutManager(float x, float y, float width, float height) : m_current(x, y, width, height) {

}

const Rect& LayoutManager::current() const {
    return m_current;
}

LayoutManager LayoutManager::copy() const {
    return LayoutManager(m_current.x, m_current.y, m_current.width, m_current.height);
}

void LayoutManager::pad(float padding) {
    pad(padding, padding);
}

void LayoutManager::pad(float paddingY, float paddingX) {
    m_current.x += paddingX;
    m_current.y += paddingY;
    m_current.width -= paddingX * 2;
    m_current.height -= paddingY * 2;

}

void LayoutManager::padLeft(float padding) {
    m_current.x += padding;
    m_current.width -= padding;
}

void LayoutManager::padRight(float padding) {
    m_current.width -= padding;
}

void LayoutManager::padUp(float padding) {
    m_current.y += padding;
    m_current.height -= padding;

}

void LayoutManager::padDown(float padding) {
    m_current.height -= padding;
}

void LayoutManager::border(float border_size) {
    m_current.x += border_size;
    m_current.y += border_size;
    m_current.width -= border_size * 2;
    m_current.height -= border_size * 2;
}

void LayoutManager::gapX(float gap_size) {
    m_current.x += gap_size;
    m_current.width -= gap_size;
}

void LayoutManager::gapY(float gap_size) {
    m_current.y += gap_size;
    m_current.height -= gap_size;
}

LayoutManager LayoutManager::sliceTop(float height) {
    LayoutManager slice(m_current.x, m_current.y, m_current.width, height);
    m_current.y += height;
    m_current.height -= height;
    return slice;
}
