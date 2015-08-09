#pragma once

#include "Player.h"

namespace Citadel
{
    class RobotPlayer : public Player
    {
    public:
        RobotPlayer();
        RobotPlayer(const std::string& name);

        virtual Character PickCharacter(const std::unordered_set<Character>& remainingCards) override;
        virtual void PlayCharacter() override;
    };
}