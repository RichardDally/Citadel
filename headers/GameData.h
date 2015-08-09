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

enum class PlayerAction
{
    UNITIALIZED,
    TAKE_GOLD_COINS,
    WATCH_DISTRICT_CARDS,
    BUILD_DISTRICT_CARDS,
    USE_MAGIC_POWER,
    MAX
};
const char* const GetPlayerActionName(const PlayerAction action);

enum class MagicianChoice
{
    UNITIALIZED,
    EXCHANGE_FROM_PLAYER,
    EXCHANGE_FROM_DISTRICT_DECK,
    DO_NOTHING,
    MAX
};
const char* const GetMagicianChoiceName(const MagicianChoice action);