#include "freetypelibrary.hpp"
#include <spdlog/spdlog.h>

FreeTypeLibrary::FreeTypeLibrary() : m_library(nullptr)
{
    if (FT_Init_FreeType(&m_library))
    {
        spdlog::error("FreeType: Could not init FreeType Library");
        m_library = nullptr;
    }
}

FreeTypeLibrary::~FreeTypeLibrary()
{
    if (m_library)
    {
        FT_Done_FreeType(m_library);
    }
}
