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

    const char* const playerActionNames[] = 
    {
        "UNITIALIZED",
        "TAKE_GOLD_COINS",
        "WATCH_DISTRICT_CARDS",
        "BUILD_DISTRICT_CARDS",
        "USE_MAGIC_POWER",
    };

    const char* const magicianChoiceNames[] =
    {
        "UNITIALIZED",
        "EXCHANGE_FROM_PLAYER",
        "EXCHANGE_FROM_DISTRICT_DECK",
        "DO_NOTHING",
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

const char* const GetPlayerActionName(const PlayerAction action)
{
    static_assert(static_cast<size_t>(PlayerAction::MAX) == sizeof(playerActionNames) / sizeof(playerActionNames[0]),
        "PlayerAction::MAX must match playerActionNames number of items");
    return playerActionNames[static_cast<size_t>(action)];
}

const char* const GetMagicianChoiceName(const MagicianChoice choice)
{
    static_assert(static_cast<size_t>(MagicianChoice::MAX) == sizeof(magicianChoiceNames) / sizeof(magicianChoiceNames[0]),
        "MagicianChoice::MAX must match magicianChoiceNames number of items");
    return magicianChoiceNames[static_cast<size_t>(choice)];
}
