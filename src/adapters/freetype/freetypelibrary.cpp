#include "freetypelibrary.hpp"
#include <stdexcept>

FreeTypeLibrary::FreeTypeLibrary() : m_library(nullptr)
{
    if (FT_Init_FreeType(&m_library))
    {
        throw std::runtime_error("FreeType: Could not init FreeType Library");
    }
}

FreeTypeLibrary::~FreeTypeLibrary()
{
    if (m_library)
    {
        FT_Done_FreeType(m_library);
    }
}
