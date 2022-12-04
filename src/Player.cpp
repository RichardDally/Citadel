#include <algorithm>
#include <spdlog/spdlog.h>
#include "Player.h"


namespace Citadel
{
    int Player::idGenerator_ = 0;

    bool Player::DestroyDistrict(const District district)
    {
        auto it = builtCity_.find(district);
        if (it != builtCity_.end())
        {
            builtCity_.erase(it);
            return true;
        }
        return false;
    }

    void Player::BuildDistrict(const std::vector<District>& districts)
    {
        for (const auto district : districts)
        {
            if (std::find(builtCity_.begin(), builtCity_.end(), district) == builtCity_.end())
            {
                auto pos = std::find(std::begin(availableDistricts_), std::end(availableDistricts_), district);
                if (pos != std::end(availableDistricts_))
                {
                    availableDistricts_.erase(pos);
                    builtCity_.insert(district);
                    ModifyGoldCoins(-GetDistrictCost(district));
                }
            }
            else
            {
                spdlog::error("District [{}] is already built. Cannot build again.", GetDistrictName(district));
            }
        }
    }

    void Player::ModifyGoldCoins(const int modifier)
    {
        goldCoins_ += modifier;
        if (goldCoins_ < 0)
        {
            assert(!"Gold coins cannot be negative");
            goldCoins_ = 0;
        }
    }
}
