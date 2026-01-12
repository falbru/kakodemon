#include "line.hpp"
#include "domain/utf8string.hpp"

namespace domain {

Line::Line() {

}

Line::Line(std::vector<Atom> atoms) : m_atoms(atoms) {

}

const Atom& Line::at(int index) const {
    return m_atoms[index];
}

Line Line::slice(int start_index, int length) {
    if (length <= 0) {
        return Line();
    }

    int i = 0;
    int size_it = 0;

    while (i < m_atoms.size() && size_it + m_atoms[i].size() <= start_index) {
        size_it += m_atoms[i].size();
        i++;
    }

    if (i == m_atoms.size()) {
        return Line();
    }

    int start_atom_index = i;
    int start_atom_start_index = start_index - size_it;

    if (m_atoms[start_atom_index].size() - start_atom_start_index >= length) {
        return Line({m_atoms[start_atom_index].slice(start_atom_start_index, length)});
    }

    int remaining_length = length - (m_atoms[start_atom_index].size() - start_atom_start_index);

    i = start_atom_index + 1;
    while (i < m_atoms.size() && remaining_length > m_atoms[i].size()) {
        remaining_length -= m_atoms[i].size();
        i++;
    }

    std::vector<Atom> line_atoms;
    line_atoms.reserve(i - start_atom_index + 1);

    Atom start_atom = m_atoms[start_atom_index].slice(start_atom_start_index);
    line_atoms.push_back(start_atom);

    if (i > start_atom_index + 1) {
        line_atoms.insert(line_atoms.end(),
                          m_atoms.begin() + start_atom_index + 1,
                          m_atoms.begin() + i);
    }

    if (i < m_atoms.size() && remaining_length > 0) {
        auto end_atom = m_atoms[i].slice(0, remaining_length);
        line_atoms.push_back(end_atom);
    }

    return Line(line_atoms);
}

const std::vector<Atom>& Line::getAtoms() const {
    return m_atoms;
}

unsigned int Line::size() const {
    return m_atoms.size();
}

unsigned int Line::length() const {
    int length = 0;
    for (int i = 0; i < m_atoms.size(); i++) {
        length += m_atoms[i].size();
    }
    return length;
}

UTF8String Line::toUTF8String() const {
    UTF8String string;
    size_t total_size = 0;
    for (int i = 0; i < m_atoms.size(); i++) {
        total_size += m_atoms[i].size();
    }
    string.reserve(total_size);
    for (int i = 0; i < m_atoms.size(); i++) {
        string += m_atoms[i].toUTF8String();
    }
    return string;
}

};
