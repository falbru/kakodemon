#ifndef DOMAIN_LINES_HPP_INCLUDED
#define DOMAIN_LINES_HPP_INCLUDED

#include "domain/line.hpp"

namespace domain
{

class Lines
{
  public:
    Lines();
    Lines(std::vector<Line> lines);

    const Line &at(int index) const;

    const std::vector<Line> &getLines() const;
    unsigned int size() const;

    std::vector<Line>::const_iterator begin() const;
    std::vector<Line>::const_iterator end() const;

  private:
    std::vector<Line> m_lines;
};

} // namespace domain

#endif
