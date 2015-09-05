#include "DistrictData.h"

namespace Citadel
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
}