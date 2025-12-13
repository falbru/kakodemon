#ifndef MODEL_UIOPTIONS_HPP_INCLUDED
#define MODEL_UIOPTIONS_HPP_INCLUDED

#include "domain/ports/font.hpp"
#include <memory>

struct UIOptions
{
    std::unique_ptr<domain::Font> font;
};

#endif
