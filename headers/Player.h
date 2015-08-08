#pragma once

#include <cassert>
#include <string>
#include <vector>

#include "GameData.h"

class Player
{
public:
    Player() = default;
    Player(const std::string& name)
        : name_(name)
    {
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

    virtual void PlayTurn() = 0;

protected:
    std::string name_;
    Character character_ = Character::UNINITIALIZED;
    std::vector<int> cardsInHand_;
    std::vector<int> builtCity_;
    int goldCoins_ = 0;
};

class HumanPlayer : public Player
{
public:
    HumanPlayer() = default;

    virtual void PlayTurn() override
    {
    }
};

class RobotPlayer : public Player
{
public:
    RobotPlayer() = default;

    virtual void PlayTurn() override
    {
    }
};