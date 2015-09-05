#pragma once

#include <vector>
#include "GameData.h"
#include "ColorData.h"

namespace Citadel
{
#pragma region Character
    enum class Character
    {
        UNINITIALIZED,
        ASSASSIN,
        THIEF,
        MAGICIAN,
        KING,
        BISHOP,
        MERCHANT,
        ARCHITECT,
        WARLORD,
        MAX
    };
    const char* const GetCharacterName(const Character character);
    const char* const GetCharacterDescription(const Character character);
    const Color GetCharacterColor(const Character character);
    const std::vector<Character>& GetCharacterCallingOrder(const Edition edition);
#pragma endregion

#pragma region MagicianChoice
    enum class MagicianChoice
    {
        UNITIALIZED,
        EXCHANGE_FROM_PLAYER,
        EXCHANGE_FROM_DISTRICT_DECK,
        DO_NOTHING,
        MAX
    };
    const char* const GetMagicianChoiceName(const MagicianChoice action);
#pragma endregion
}