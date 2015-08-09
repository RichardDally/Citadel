#pragma once

#include "Player.h"

namespace Citadel
{
    class HumanPlayer : public Player
    {
    public:
        HumanPlayer();
        HumanPlayer(const std::string& name);

        virtual Character PickCharacter(const std::unordered_set<Character>& remainingCards) override;
        virtual void PlayCharacter() override;
    };
}