#include <cstddef> // size_t definition
#include "GameData.h"

namespace Citadel
{
    namespace
    {
        const char* const editionNames[] =
        {
            "UNINITIALIZED",
            "REGULAR",
            "REGULAR WITHOUT PURPLE DISTRICTS",
            "DARK CITY"
        };
    }

    const char* const GetEditionName(const Edition edition)
    {
        static_assert(static_cast<size_t>(Edition::MAX) == sizeof(editionNames) / sizeof(editionNames[0]),
            "Edition::MAX must match editionNames number of items");
        return editionNames[static_cast<size_t>(edition)];
    }
}