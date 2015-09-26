#pragma once

#include <vector>
#include "DistrictData.h"

namespace Citadel
{
    enum class DistrictDeckAction
    {
        PEEK,
        DRAW
    };

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

        std::vector<District> GetDistricts(const DistrictDeckAction action, const size_t n);

        // Put district(s) below the bottom of pile
        void Discard(const std::vector<District>& districts);

    private:
        std::vector<District> pileOfCards_;
    };
}