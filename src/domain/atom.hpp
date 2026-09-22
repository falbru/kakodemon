#ifndef DOMAIN_ATOM_HPP_INCLUDED
#define DOMAIN_ATOM_HPP_INCLUDED

#include "codepointstring.hpp"
#include "domain/face.hpp"

namespace domain
{

class Atom
{
  public:
    Atom(CodepointString contents, Face face);

    const CodepointString &getContents() const;
    const Face &getFace() const;
    unsigned int size() const;

    Atom slice(int start_index);
    Atom slice(int start_index, int length);

    Atom trim(TrimDirection direction = TrimDirection::Both) const;

    CodepointString toCodepointString() const;

  private:
    CodepointString m_contents;
    Face m_face;
};

} // namespace domain

#endif
