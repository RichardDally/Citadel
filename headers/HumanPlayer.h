#pragma once

#include "Player.h"

namespace Citadel
{
    class HumanPlayer : public Player
    {
    public:
        HumanPlayer();
        HumanPlayer(const std::string& name);

        virtual Character PickRoleDecision(const std::unordered_set<Character>& remainingCards) override;
        virtual void PlayRoleStep() override;
    };
}