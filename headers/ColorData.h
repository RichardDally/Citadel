#pragma once

#include <cstddef> // size_t definition

namespace Citadel
{
    enum class Color
    {
        UNINITIALIZED,
        RED, // Warlord/Diplomat district
        GREEN, // Merchant district
        YELLOW,  // King/Emperor district
        BLUE, // Bishop/Abbot district
        PURPLE, // Special district
        MAX
    };
    const char* const GetColorName(const Color character);
    const size_t GetColorNumber();
}