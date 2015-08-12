#pragma once

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

        const size_t GetPileOfCardSize() const
        {
            return pileOfCards_.size();
        }

        // Pick a district card from top of the stack
        std::vector<District> Draw(const size_t n);

        // Put a district card below the bottom of the stack
        void Discard(const District district);

    private:
        std::vector<District> pileOfCards_;
    };
}