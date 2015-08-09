#pragma once

#include "Player.h"

namespace Citadel
{
    class RobotPlayer : public Player
    {
    public:
        RobotPlayer();
        RobotPlayer(const std::string& name);

        virtual Character PickRoleDecision(const std::unordered_set<Character>& remainingCards) override;
        virtual void PlayRoleStep() override;
    };
}