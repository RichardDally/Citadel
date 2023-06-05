#include <random>
#include <vector>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <spdlog/spdlog.h>
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

        std::random_device randomDevice;
        std::mt19937 randomNumberGenerator(randomDevice());

        // Once all cards have been pushed, shuffle them
        std::shuffle(std::begin(pileOfCards_), std::end(pileOfCards_), randomNumberGenerator);
    }

    std::vector<District> DistrictDeck::GetDistricts(const DistrictDeckAction action, const size_t n)
    {
        std::vector<District> result;

        spdlog::debug("Remaining cards [{}]", pileOfCards_.size());
        if (pileOfCards_.size() >= n)
        {
            result.assign(std::begin(pileOfCards_), std::begin(pileOfCards_) + n);
            if (action == DistrictDeckAction::DRAW)
            {
                pileOfCards_.erase(std::begin(pileOfCards_), std::begin(pileOfCards_) + n);
            }
        }
        else
        {
            spdlog::critical("There is not enough districts.");
            assert(!"There is not enough districts.");
        }

        return result;
    }

    void DistrictDeck::Discard(const std::vector<District>& districts)
    {
        pileOfCards_.insert(std::end(pileOfCards_), std::begin(districts), std::end(districts));
    }
}
