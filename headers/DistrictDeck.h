#pragma once

#include <map>
#include <vector>
#include "GameData.h"

namespace Citadel
{
    class DistrictDeck
    {
    public:
        DistrictDeck() = default;
        ~DistrictDeck() = default;
        DistrictDeck(const DistrictDeck&) = default;
        DistrictDeck& operator=(const DistrictDeck&) = default;

        void Setup(const std::vector<District>& availableDistricts);

    private:
        std::map<District, size_t> availableCards_;
        std::vector<District> heapOfCards_;
    };
}