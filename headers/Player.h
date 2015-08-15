#pragma once

#include <set>
#include <map>
#include <string>
#include <vector>
#include <cassert>
#include <iostream>

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

    std::vector<District>& GetCardsInHand()
    {
        return cardsInHand_;
    }

    const std::set<District>& GetBuiltCity() const
    {
        return builtCity_;
    }

    void BuildDistrict(const std::vector<District>& districts)
    {
        for (const auto district : districts)
        {
            if (std::find(builtCity_.begin(), builtCity_.end(), district) == builtCity_.end())
            {
                auto pos = std::find(cardsInHand_.begin(), cardsInHand_.end(), district);
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

#pragma region PURE VIRTUAL METHODS
    // Returns character picked to play
    virtual Character PickCharacter(const std::set<Character>& remainingCards) = 0;

    // Returns action to be taken
    virtual PlayerAction ChooseAction() = 0;

    // Watch proposed district cards and return selected card
    virtual District WatchAndChooseDistrictCard(const std::vector<District>& districts) = 0;

    // Returns district card id player wants to build
    virtual std::vector<District> ChooseDistrictCardsToBuild(const size_t authorizedBuilds) = 0;

    // Returns character targeted by assassination or theft
    virtual Character ChooseCharacterTarget(const std::set<Character>& opponents) = 0;

    // Returns opponent player id, current player wants to target
    virtual int ChoosePlayerTarget(std::vector<const Player*> opponents) = 0;

    // Returns a choice specific to Magician character
    virtual MagicianChoice MagicianDecision() = 0;

    // Returns discarded districts to be replaced by equivalent number of cards from district deck
    virtual std::vector<District> ChooseDistrictsCardsToSwap() = 0;
#pragma endregion

protected:
    Character character_ = Character::UNINITIALIZED;
    std::string name_;

    // Key: district card id
    std::vector<District> cardsInHand_;

    // District card id
    // using std::set because you cannot build twice same district
    std::set<District> builtCity_;

    int goldCoins_ = 0;

private:
    static int idGenerator_;
    int id_ = idGenerator_++;
};
