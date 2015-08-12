#include <vector>
#include <cassert>
#include <iostream>
#include <algorithm>
#include "DistrictDeck.h"

namespace Citadel
{
    void DistrictDeck::Setup(const std::vector<District>& availableDistricts)
    {
        for (const auto district : availableDistricts)
        {
            const size_t districtQuantity = GetDistrictQuantity(district);
            for (size_t i = 0; i < districtQuantity; ++i)
            {
                pileOfCards_.push_back(district);
            }
        }

        // Once all cards have been pushed, shuffle them
        std::random_shuffle(std::begin(pileOfCards_), std::end(pileOfCards_));
    }

    // Pick n district card from top of the stack
    std::vector<District> DistrictDeck::Draw(const size_t n)
    {
        std::vector<District> result;

        if (pileOfCards_.size() >= n)
        {
            result.assign(std::begin(pileOfCards_), std::begin(pileOfCards_) + n);
            pileOfCards_.erase(std::begin(pileOfCards_), std::begin(pileOfCards_) + n);
        }
        else
        {
            assert(!"There is not enough cards to draw");
        }

        return result;
    }

    // Put a district card below the bottom of the stack
    void DistrictDeck::Discard(const std::vector<District>& districts)
    {
        pileOfCards_.insert(std::end(pileOfCards_), std::begin(districts), std::end(districts));
    }
}