#pragma once

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

enum class Color
{
    UNINITIALIZED,
    RED, // Warlord district
    GREEN, // Merchant district
    YELLOW,  // King district
    BLUE, // Bishop district
    PURPLE, // Special district
    MAX
};
const char* const GetColorName(const Color character);