#include <vector>
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

    // Pick a district card from top of the stack
    District DistrictDeck::Draw()
    {
        District district = District::UNINITIALIZED;

        if (pileOfCards_.size() > 0)
        {
            district = pileOfCards_.front();
            pileOfCards_.erase(std::begin(pileOfCards_));
        }

        return district;
    }

    // Put a district card below the bottom of the stack
    void DistrictDeck::Discard(const District district)
    {
        pileOfCards_.push_back(district);
    }
}