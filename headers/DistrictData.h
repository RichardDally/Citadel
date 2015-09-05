#pragma once

#include "ColorData.h"

#pragma region District
namespace Citadel
{
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
#pragma region PURPLE DISTRICTS
        HAUNTED_CITY,
        KEEP,
        LABORATORY,
        SMITHY,
        OBSERVATORY,
        GRAVEYARD,
        DRAGON_GATE,
        UNIVERSITY,
        LIBRARY,
        GREAT_WALL,
        SCHOOL_OF_MAGIC,
#pragma endregion
        MAX
    };
    const char* const GetDistrictName(const District district);
    const char* const GetDistrictDescription(const District district);
    const Color       GetDistrictColor(const District district);
    const int         GetDistrictCost(const District district);
    const size_t      GetDistrictQuantity(const District district);
}
#pragma endregion