#include "ColorData.h"

namespace Citadel
{
    namespace
    {
        const char* const colorNames[] =
        {
            "UNINITIALIZED",
            "RED",
            "GREEN",
            "YELLOW",
            "BLUE",
            "PURPLE",
        };
    }

    const char* const GetColorName(const Color color)
    {
        static_assert(static_cast<size_t>(Color::MAX) == sizeof(colorNames) / sizeof(colorNames[0]),
            "Color::MAX must match colorNames number of items");
        return colorNames[static_cast<size_t>(color)];
    }

    const size_t GetColorNumber()
    {
        // Minus 1 to remove Color::UNINITIALIZED
        return static_cast<size_t>(Color::MAX) - 1;
    }
}