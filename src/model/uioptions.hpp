#ifndef MODEL_UIOPTIONS_HPP_INCLUDED
#define MODEL_UIOPTIONS_HPP_INCLUDED

#include "font.hpp"
#include <memory>

struct UIOptions
{
    std::unique_ptr<Font> font;
};

#endif
