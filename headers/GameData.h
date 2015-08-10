#pragma once

// Base game characters
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

// Base game districts
enum class District
{
    UNINITIALIZED,
#pragma region RED DISTRICTS
    WATCHTOWER,
    PRISON,
    BATTLEFIELD,
    FORTRESS,
#pragma endregion
#pragma region YELLOW DISTRICTS
    MANOR,
    CASTLE,
    PALACE,
#pragma endregion
#pragma region GREEN DISTRICTS
    TAVERN,
    MARKET,
    TRADING_POST,
    DOCKS,
    HARBOR,
    TOWN_HALL,
#pragma endregion
#pragma region BLUE DISTRICTS
    TEMPLE,
    CHURCH,
    MONASTARY,
    CATHEDRAL,
#pragma endregion
    // TODO: Enable PURPLE DISTRICTS (update game rules)
#pragma region PURPLE DISTRICTS
    //HAUNTED_CITY,
    //KEEP,
    //LABORATORY,
    //SMITHY,
    //OBSERVATORY,
    //GRAVEYARD,
    //DRAGON_GATE,
    //UNIVERSITY,
    //LIBRARY,
    //GREAT_WALL,
    //SCHOOL_OF_MAGIC,
#pragma endregion
    MAX
};
const char* const GetDistrictName(const District district);
const char* const GetDistrictDescription(const District district);
const int         GetDistrictCost(const District district);
const size_t      GetDistrictQuantity(const District district);

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