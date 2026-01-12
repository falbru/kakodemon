#ifndef DOMAIN_LINE_HPP_INCLUDED
#define DOMAIN_LINE_HPP_INCLUDED

#include "domain/atom.hpp"
#include "domain/utf8string.hpp"

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

    UTF8String toUTF8String() const;

  private:
    std::vector<Atom> m_atoms;
};

} // namespace domain

#endif
