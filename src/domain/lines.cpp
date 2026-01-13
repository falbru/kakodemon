#include "lines.hpp"
#include "domain/line.hpp"

namespace domain {

Lines::Lines() {

}

Lines::Lines(std::vector<Line> lines) : m_lines(lines) {

}

const Line& Lines::at(int index) const {
    return m_lines.at(index);
}

const std::vector<Line>& Lines::getLines() const {
    return m_lines;
}

unsigned int Lines::size() const {
    return m_lines.size();
}

std::vector<Line>::const_iterator Lines::begin() const {
    return m_lines.begin();
}

std::vector<Line>::const_iterator Lines::end() const {
    return m_lines.end();
}
}
