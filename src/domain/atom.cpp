#include "atom.hpp"
#include "domain/utf8string.hpp"

namespace domain {

Atom::Atom(UTF8String contents, Face face) : m_contents(contents), m_face(face) {

}

const UTF8String& Atom::getContents() const {
    return m_contents;
}

const Face& Atom::getFace() const {
    return m_face;
}

unsigned int Atom::size() const {
    return m_contents.size();
}

Atom Atom::slice(int start_index) {
    return slice(start_index, size() - start_index);
}

Atom Atom::slice(int start_index, int length) {
    if (start_index < 0 || start_index >= m_contents.size() || length < 0)
        return Atom({}, m_face);

    return Atom(m_contents.substring(start_index, length), m_face);
}

UTF8String Atom::toUTF8String() const {
    return UTF8String(m_contents);
}

}
