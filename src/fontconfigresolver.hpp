#ifndef FONTCONFIGRESOLVER_HPP_INCLUDED
#define FONTCONFIGRESOLVER_HPP_INCLUDED

#include "fontresolver.hpp"

class FontconfigResolver : public FontResolver
{
  public:
    FontconfigResolver();
    ~FontconfigResolver();

    FontMatch resolve(std::string pattern) override;
};

#endif
