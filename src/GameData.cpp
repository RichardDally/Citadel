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
            "Character::MAX must match characterNames number of items");
        return editionNames[static_cast<size_t>(edition)];
    }
}