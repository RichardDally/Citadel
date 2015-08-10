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
            if (GetDistrictQuantity(district) > 0)
            {
                availableCards_.insert(std::make_pair(district, GetDistrictQuantity(district)));
                heapOfCards_.push_back(district);
            }
            else
            {
                std::cout << "District [" << GetDistrictName(district) << "] is not available." << std::endl;
            }
        }

        // Once all cards have been pushed, shuffle them
        std::random_shuffle(std::begin(heapOfCards_), std::end(heapOfCards_));
    }
}