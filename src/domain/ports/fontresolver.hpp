#ifndef DOMAIN_FONTRESOLVER_HPP_INCLUDED
#define DOMAIN_FONTRESOLVER_HPP_INCLUDED

#include <string>

namespace domain
{

struct FontMatch
{
    std::string path;
    int size;
};

class FontResolver
{
  public:
    virtual ~FontResolver() = default;

    virtual FontMatch resolve(std::string pattern) = 0;
};

} // namespace domain

#endif
