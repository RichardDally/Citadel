#include <cstddef> // sizeof operator
#include "GameData.h"

namespace
{
    const char* const characterNames[] =
    {
        "UNINITIALIZED",
        "ASSASSIN",
        "THIEF",
        "MAGICIAN",
        "KING",
        "BISHOP",
        "MERCHANT",
        "ARCHITECT",
        "WARLORD"
    };

    // TODO: fill descriptions
    const char* const characterDescriptions[] =
    {
        "No description",
        "ASSASSIN description",
        "THIEF description",
        "MAGICIAN description",
        "KING description",
        "BISHOP description",
        "MERCHANT description",
        "ARCHITECT description",
        "WARLORD description"
    };

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

const char* const GetCharacterName(const Character character)
{
    static_assert(static_cast<size_t>(Character::MAX) == sizeof(characterNames) / sizeof(characterNames[0]),
        "Character::MAX must match characterNames number of items");
    return characterNames[static_cast<size_t>(character)];
}

const char* const GetCharacterDescription(const Character character)
{
    static_assert(static_cast<size_t>(Character::MAX) == sizeof(characterDescriptions) / sizeof(characterDescriptions[0]),
        "Character::MAX must match characterDescriptions number of items");
    return characterDescriptions[static_cast<size_t>(character)];
}

const char* const GetColorName(const Color color)
{
    static_assert(static_cast<size_t>(Color::MAX) == sizeof(colorNames) / sizeof(colorNames[0]),
        "Color::MAX must match colorNames number of items");
    return colorNames[static_cast<size_t>(color)];
}

