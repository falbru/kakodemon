#ifndef FONTCONFIGRESOLVER_HPP_INCLUDED
#define FONTCONFIGRESOLVER_HPP_INCLUDED

#include "domain/ports/fontresolver.hpp"

class FontconfigResolver : public domain::FontResolver
{
  public:
    FontconfigResolver();
    ~FontconfigResolver();

    domain::FontMatch resolve(const std::string &pattern) override;
    domain::FontMatch resolveDefault(int size) override;
    std::optional<domain::FontMatch> resolveForCodepoint(domain::Codepoint codepoint) override;
    std::optional<domain::FontMatch> resolveStyleVariant(const domain::FontMatch &base_font,
                                                         domain::FontStyle style) override;
};

#endif
