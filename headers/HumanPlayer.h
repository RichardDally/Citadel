#pragma once

#include "Player.h"

class HumanPlayer : public Player
{
public:
    HumanPlayer() = default;

    virtual Character PickRoleDecision(const std::unordered_set<Character>& remainingCards) override
    {
        assert(remainingCards.size() > 0);
        return *std::begin(remainingCards);
    }

    virtual void PlayRoleStep() override
    {
    }
};