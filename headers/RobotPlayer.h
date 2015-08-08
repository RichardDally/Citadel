#pragma once

#include "Player.h"

class RobotPlayer : public Player
{
public:
    RobotPlayer() = default;

    virtual Character PickRoleDecision(const std::unordered_set<Character>& remainingCards) override
    {
        assert(remainingCards.size() > 0);
        return *std::begin(remainingCards);
    }

    virtual void PlayRoleStep() override
    {
    }
};