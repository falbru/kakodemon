#ifndef DOMAIN_LINE_HPP_INCLUDED
#define DOMAIN_LINE_HPP_INCLUDED

#include "domain/atom.hpp"
#include "domain/codepointstring.hpp"

namespace domain
{

class Line
{
  public:
    Line();
    Line(std::vector<Atom> m_atoms);

    const Atom &at(int index) const;
    Line slice(int start_index, int length);

    const std::vector<Atom> &getAtoms() const;
    unsigned int size() const;
    unsigned int length() const;

    Line trim(TrimDirection direction = TrimDirection::Both) const;

    CodepointString toCodepointString() const;

  private:
    std::vector<Atom> m_atoms;
};

} // namespace domain

#endif
