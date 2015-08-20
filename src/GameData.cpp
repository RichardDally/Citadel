#include <cassert>
#include <iostream>
#include "GameData.h"

#pragma region Color
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
#pragma endregion

#pragma region Character
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

    const char* const characterDescriptions[] =
    {
        "No description",
        "Tremendously handy for shanking those greedy Merchants who try to get rich quick",
        "Why work hard to amass gold when you could just steal it ?",
        "Like a certain Spanish Inquisition, no one ever expects the Magician.",
        "Looking down on you with snooty horror, the King wonders why you've dared invade his chambers with your filthy person, one hand poised to make a dismissive \"flicking\" motion in your general direction.",
        "Practically a license to print money for the enterprising builder with lots of religious districts in his city.",
        "Merchant exists to fill the coffers of the enterprising player, and he does it very well.",
        "There's no better way to build up a monster hand of cards than with the Architect.",
        "Smug and confident in his ability to kill you at pretty much any moment, the Warlord sits back and relaxes as he ponders how best to destroy you."
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

const Color GetCharacterColor(const Character character)
{
    Color result = Color::UNINITIALIZED;

    switch (character)
    {
        case Character::ASSASSIN: break;
        case Character::THIEF: break;
        case Character::MAGICIAN: break;
        case Character::KING:
        {
            result = Color::YELLOW;
            break;
        }
        case Character::BISHOP:
        {
            result = Color::BLUE;
            break;
        }
        case Character::MERCHANT:
        {
            result = Color::GREEN;
            break;
        }
        case Character::ARCHITECT: break;
        case Character::WARLORD:
        {
            result = Color::RED;
            break;
        }
        default:
        {
            assert(!"This character is not handled to get it's color");
            std::cerr << "Character [" << static_cast<int>(character) << "] is not handled." << std::endl;
        }
    }

    return result;
}
#pragma endregion

#pragma region District
namespace
{
    struct DistrictData
    {
        const char* const name_;
        const char* const description_;
        const Color       color_;
        const int         cost_;
        const size_t      quantity_;
    };

    // TODO: fill descriptions
    const DistrictData districtMetaData[] = 
    {
        { "UNINITIALIZED", "UNINITIALIZED description", Color::UNINITIALIZED, -1, 0 },

#pragma region RED DISTRICTS
        { "WATCHTOWER", "WATCHTOWER description", Color::RED, 1, 3 },
        { "PRISON", "PRISON description", Color::RED, 2, 3 },
        { "BATTLEFIELD", "BATTLEFIELD description", Color::RED, 3, 3 },
        { "FORTRESS", "FORTRESS description", Color::RED, 5, 2 },
#pragma endregion

#pragma region YELLOW DISTRICTS
        { "MANOR", "MANOR description", Color::YELLOW, 3, 5 },
        { "CASTLE", "CASTLE description", Color::YELLOW, 4, 4 },
        { "PALACE", "PALACE description", Color::YELLOW, 5, 3 },
#pragma endregion

#pragma region GREEN DISTRICTS
        { "TAVERN", "TAVERN description", Color::GREEN, 1, 5 },
        { "MARKET", "MARKET description", Color::GREEN, 2, 4 },
        { "TRADING_POST", "TRADING_POST description", Color::GREEN, 2, 3 },
        { "DOCKS", "DOCKS description", Color::GREEN, 3, 3 },
        { "HARBOR", "HARBOR description", Color::GREEN, 4, 3 },
        { "TOWN_HALL", "TOWN_HALL description", Color::GREEN, 5, 2 },
#pragma endregion

#pragma region BLUE DISTRICTS
        { "TEMPLE", "TEMPLE description", Color::BLUE, 1, 3 },
        { "CHURCH", "CHURCH description", Color::BLUE, 2, 3 },
        { "MONASTARY", "MONASTARY description", Color::BLUE, 3, 3 },
        { "CATHEDRAL", "CATHEDRAL description", Color::BLUE, 5, 2 },
#pragma endregion

#pragma region PURPLE DISTRICTS
        // TODO: implement rules to deal with purple districts
        { "HAUNTED_CITY", "For the purposes of victory points, the Haunted City is conisdered to be of the color of your choice. You cannot use this ability if you built it during the last round of the game.", Color::PURPLE, 2, 1 },
        { "KEEP", "The Keep cannot be destroyed by the Warlord", Color::PURPLE, 3, 2 },
        { "LABORATORY", "Once during your turn, you may discard a district card from your hand and receive one gold from the bank", Color::PURPLE, 5, 1 },
        { "SMITHY", "Once during your turn, you may pay two gold to draw 3 district cards.", Color::PURPLE, 5, 1 },
        { "OBSERVATORY", "If you choose to draw cards when you take an action, you draw 3 cards, keep one of your choice, and put the other 2 on the bottom of the deck", Color::PURPLE, 5, 1 },
        { "GRAVEYARD", "When the Warlord destroys a district, you may pay one gold to take the destroyed district into your hand.  You may not do this if you are the Warlord", Color::PURPLE, 5, 1 },
        { "DRAGON_GATE", "This district costs 6 gold to build, but is worth 8 points at the end of the game", Color::PURPLE, 6, 1 },
        { "UNIVERSITY", "This district costs 6 gold to build, but is worth 8 points at the end of the game", Color::PURPLE, 6, 1 },
        { "LIBRARY", "If you choose to draw cards you you take an action, you keep both of the cards you have drawn.", Color::PURPLE, 6, 1 },
        { "GREAT_WALL", "The cost for the Warlord to destory any of your other districts is increased by one gold", Color::PURPLE, 6, 1 },
        { "SCHOOL_OF_MAGIC", "This district costs 6 gold to build, but is worth 8 points at the end of the game", Color::PURPLE, 6, 1 },
#pragma endregion
    };
}

const char* const GetDistrictName(const District district)
{
    static_assert(static_cast<size_t>(District::MAX) == sizeof(districtMetaData) / sizeof(districtMetaData[0]),
        "District::MAX must match districtMetaData number of items");
    return districtMetaData[static_cast<size_t>(district)].name_;
}

const char* const GetDistrictDescription(const District district)
{
    static_assert(static_cast<size_t>(District::MAX) == sizeof(districtMetaData) / sizeof(districtMetaData[0]),
        "District::MAX must match districtMetaData number of items");
    return districtMetaData[static_cast<size_t>(district)].description_;
}

const Color GetDistrictColor(const District district)
{
    static_assert(static_cast<size_t>(District::MAX) == sizeof(districtMetaData) / sizeof(districtMetaData[0]),
        "District::MAX must match districtMetaData number of items");
    return districtMetaData[static_cast<size_t>(district)].color_;
}

const int GetDistrictCost(const District district)
{
    static_assert(static_cast<size_t>(District::MAX) == sizeof(districtMetaData) / sizeof(districtMetaData[0]),
        "District::MAX must match districtMetaData number of items");
    return districtMetaData[static_cast<size_t>(district)].cost_;
}

const size_t GetDistrictQuantity(const District district)
{
    static_assert(static_cast<size_t>(District::MAX) == sizeof(districtMetaData) / sizeof(districtMetaData[0]),
        "District::MAX must match districtMetaData number of items");
    return districtMetaData[static_cast<size_t>(district)].quantity_;
}
#pragma endregion

#pragma region PlayerTurnStep
namespace
{
    const char* const playerTurnStepNames[] =
    {
        "UNINITIALIZED",
        "ACTION_STEP",
        "BUILD_STEP",
        "MAGIC_POWER_STEP",
        "ENDING_STEP",
    };
}

const char* const GetPlayerTurnStepName(const PlayerTurnStep step)
{
    static_assert(static_cast<size_t>(PlayerTurnStep::MAX) == sizeof(playerTurnStepNames) / sizeof(playerTurnStepNames[0]),
        "PlayerTurnStep::MAX must match playerTurnStepNames number of items");
    return playerTurnStepNames[static_cast<size_t>(step)];
}
#pragma endregion

#pragma region PlayerAction
namespace
{
    const char* const playerActionNames[] =
    {
        "UNITIALIZED",
        "TAKE_GOLD_COINS",
        "WATCH_DISTRICT_CARDS",
        "BUILD_DISTRICT_CARDS",
        "USE_MAGIC_POWER",
    };
}

const char* const GetPlayerActionName(const PlayerAction action)
{
    static_assert(static_cast<size_t>(PlayerAction::MAX) == sizeof(playerActionNames) / sizeof(playerActionNames[0]),
        "PlayerAction::MAX must match playerActionNames number of items");
    return playerActionNames[static_cast<size_t>(action)];
}
#pragma endregion

#pragma region MagicianChoice
namespace
{
    const char* const magicianChoiceNames[] =
    {
        "UNITIALIZED",
        "EXCHANGE_FROM_PLAYER",
        "EXCHANGE_FROM_DISTRICT_DECK",
        "DO_NOTHING",
    };
}

const char* const GetMagicianChoiceName(const MagicianChoice choice)
{
    static_assert(static_cast<size_t>(MagicianChoice::MAX) == sizeof(magicianChoiceNames) / sizeof(magicianChoiceNames[0]),
        "MagicianChoice::MAX must match magicianChoiceNames number of items");
    return magicianChoiceNames[static_cast<size_t>(choice)];
}
#pragma endregion
