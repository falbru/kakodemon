#ifndef DOMAIN_ATOM_HPP_INCLUDED
#define DOMAIN_ATOM_HPP_INCLUDED

#include "domain/face.hpp"
#include "utf8string.hpp"

namespace domain
{

class Atom
{
  public:
    Atom(UTF8String contents, Face face);

    const UTF8String &getContents() const;
    const Face &getFace() const;
    unsigned int size() const;

    Atom slice(int start_index);
    Atom slice(int start_index, int length);

    UTF8String toUTF8String() const;

  private:
    UTF8String m_contents;
    Face m_face;
};

} // namespace domain

#endif
