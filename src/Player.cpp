#include <algorithm>
#include "Player.h"

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
            auto pos = std::find(std::begin(cardsInHand_), std::end(cardsInHand_), district);
            if (pos != std::end(cardsInHand_))
            {
                cardsInHand_.erase(pos);
                builtCity_.insert(district);
                ModifyGoldCoins(-GetDistrictCost(district));
            }
        }
        else
        {
            std::cerr << "District [" << GetDistrictName(district) << "] is already built. Cannot build again." << std::endl;
        }
    }
}

void Player::ModifyGoldCoins(const int modifier)
{
    goldCoins_ += modifier;
    if (modifier < 0 && goldCoins_ + modifier < 0)
    {
        assert(!"Gold coins cannot be negative");
        goldCoins_ = 0;
    }
}