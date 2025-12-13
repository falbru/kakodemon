#ifndef FONTCONFIGRESOLVER_HPP_INCLUDED
#define FONTCONFIGRESOLVER_HPP_INCLUDED

#include "domain/ports/fontresolver.hpp"

class FontconfigResolver : public domain::FontResolver
{
  public:
    FontconfigResolver();
    ~FontconfigResolver();

    domain::FontMatch resolve(std::string pattern) override;
};

#endif
