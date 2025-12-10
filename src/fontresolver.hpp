#ifndef FONTRESOLVER_HPP_INCLUDED
#define FONTRESOLVER_HPP_INCLUDED

#include <string>

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

#endif
