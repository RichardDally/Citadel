#pragma once

#include <map>
#include <string>
#include <vector>
#include <cassert>
#include <set>

#include "GameData.h"

class Player
{
public:
    Player() = default;
    Player(const std::string& name)
        : name_(name)
    {
    }

    const Character GetCharacter() const
    {
        return character_;
    }

    void SetCharacter(const Character character)
    {
        character_ = character;
    }

    const std::string& GetName() const
    {
        return name_;
    }

    const size_t GetNumberOfCardsInHand() const
    {
        return cardsInHand_.size();
    }

    std::vector<int>& GetCardsInHand()
    {
        return cardsInHand_;
    }

    const std::vector<int>& GetBuiltCity() const
    {
        return builtCity_;
    }

    const int GetGoldCoins() const
    {
        return goldCoins_;
    }

    void ModifyGoldCoins(const int modifier)
    {
        goldCoins_ += modifier;
        if (modifier < 0 && goldCoins_ + modifier < 0)
        {
            assert(!"Gold coins cannot be negative");
            goldCoins_ = 0;
        }
    }

    int GetID() const
    {
        return id_;
    }

    // Returns character picked to play
    virtual Character PickCharacter(const std::set<Character>& remainingCards) = 0;

    // Returns action to be taken
    virtual PlayerAction ChooseAction() = 0;

    // Returns district card id player wants to build
    virtual std::vector<int> ChooseDistrictCardsToBuild(const size_t authorizedBuilds) = 0;

    // Returns character targeted by assassination or theft
    virtual Character ChooseCharacterTarget() = 0;

    // Returns opponent player id, current player wants to target
    virtual int ChoosePlayerTarget(std::vector<const Player*> opponents) = 0;

    // Returns a choice specific to Magician character
    virtual MagicianChoice MagicianDecision() = 0;

protected:
    Character character_ = Character::UNINITIALIZED;
    std::string name_;

    // Key: district card id
    std::vector<int> cardsInHand_;

    // District card id
    std::vector<int> builtCity_;

    int goldCoins_ = 0;

private:
    static int idGenerator_;
    int id_ = idGenerator_++;
};
