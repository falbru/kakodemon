#ifndef FONTCONFIGRESOLVER_HPP_INCLUDED
#define FONTCONFIGRESOLVER_HPP_INCLUDED

#include "domain/ports/fontresolver.hpp"

class FontconfigResolver : public domain::FontResolver
{
  public:
    FontconfigResolver();
    ~FontconfigResolver();

    domain::FontMatch resolve(const std::string &pattern) override;
    std::optional<domain::FontMatch> resolveForCodepoint(domain::Codepoint codepoint) override;
};

#endif
