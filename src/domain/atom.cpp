#include "atom.hpp"
#include "domain/codepointstring.hpp"

namespace domain
{

Atom::Atom(CodepointString contents, Face face) : m_contents(contents), m_face(face)
{
}

const CodepointString &Atom::getContents() const
{
    return m_contents;
}

const Face &Atom::getFace() const
{
    return m_face;
}

unsigned int Atom::size() const
{
    return m_contents.size();
}

Atom Atom::slice(int start_index)
{
    return slice(start_index, size() - start_index);
}

Atom Atom::slice(int start_index, int length)
{
    if (start_index < 0 || start_index >= m_contents.size() || length < 0)
        return Atom({}, m_face);

    return Atom(m_contents.substring(start_index, length), m_face);
}

Atom Atom::trim(TrimDirection direction) const
{
    return Atom(m_contents.trim(direction), m_face);
}

CodepointString Atom::toCodepointString() const
{
    return CodepointString(m_contents);
}

} // namespace domain
