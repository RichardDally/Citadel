#pragma once

#include <cassert>
#include <string>
#include <vector>
#include <unordered_set>

#include "GameData.h"

class Player
{
public:
    Player() = default;
    Player(const std::string& name)
        : name_(name)
    {
    }

    void SetCharacter(const Character character)
    {
        character_ = character;
    }

    const std::string& GetName() const
    {
        return name_;
    }

    const int GetGoldCoins() const
    {
        return goldCoins_;
    }

    void ModifyGoldCoins(int coins)
    {
        goldCoins_ += coins;
        if (coins < 0 && goldCoins_ + coins < 0)
        {
            assert("Gold coins cannot be negative");
            goldCoins_ = 0;
        }
    }

    virtual void PlayRoleStep() = 0;

    const Character PickRoleStep(const std::unordered_set<Character>& remainingCards)
    {
        return PickRoleDecision(remainingCards);
    }
    virtual Character PickRoleDecision(const std::unordered_set<Character>& remainingCards) = 0;

protected:
    std::string name_;
    std::vector<int> cardsInHand_;
    std::vector<int> builtCity_;
    int goldCoins_ = 0;

private:
    Character character_ = Character::UNINITIALIZED;
};
