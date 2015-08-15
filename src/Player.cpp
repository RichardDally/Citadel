#include <algorithm>
#include "Player.h"

int Player::idGenerator_ = 0;

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