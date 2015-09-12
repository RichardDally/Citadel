#pragma once

#include <set>
#include <map>
#include <string>
#include <vector>
#include <cassert>
#include <iostream>

#include "PlayerData.h"
#include "DistrictData.h"
#include "CharacterData.h"

namespace Citadel
{
    class Player
    {
    public:
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

        const size_t GetNumberOfAvailableDistricts() const
        {
            return availableDistricts_.size();
        }

        std::vector<District>& GetAvailableDistricts()
        {
            return availableDistricts_;
        }

        const size_t GetBuiltCitySize() const
        {
            return builtCity_.size();
        }

        const std::set<District>& GetBuiltCity() const
        {
            return builtCity_;
        }

        const int GetGoldCoins() const
        {
            return goldCoins_;
        }

        int GetID() const
        {
            return id_;
        }

        bool DestroyDistrict(const District district);
        void BuildDistrict(const std::vector<District>& districts);
        void ModifyGoldCoins(const int modifier);


#pragma region PURE VIRTUAL METHODS
        // Returns character picked to play
        virtual Character PickCharacter(const std::set<Character>& remainingCharacters) = 0;

        // Returns action to be taken
        virtual PlayerAction ChooseAction(const std::vector<PlayerAction>& availableActions) = 0;

        // Watch proposed district cards and return selected card
        virtual District WatchAndChooseDistrictCard(const std::vector<District>& districts) = 0;

        // Returns district card id player wants to build
        virtual std::vector<District> ChooseDistrictCardsToBuild(const size_t authorizedBuilds) = 0;

        // Returns character targeted by assassination or theft
        virtual Character ChooseCharacterTarget(const std::set<Character>& opponents) = 0;

        // Returns opponent player id, current player wants to target
        virtual int ChoosePlayerTarget(std::vector<const Player*> opponents) = 0;

        // Returns a pair containing player id (self district destroy is tolerated) as key and destroyed district as value
        virtual std::pair<int, District> ChoosePlayerDistrictTarget(std::vector<const Player*> players) = 0;

        // Returns a choice specific to Magician character
        virtual MagicianChoice MagicianDecision() = 0;

        // Returns discarded districts to be replaced by equivalent number of cards from district deck
        virtual std::vector<District> ChooseDistrictsCardsToSwap() = 0;
#pragma endregion

    protected:
        Character character_ = Character::UNINITIALIZED;
        std::string name_;

        // Key: district card id
        std::vector<District> availableDistricts_;

        // District card id
        // using std::set because you cannot build twice same district
        std::set<District> builtCity_;

        int goldCoins_ = 0;

    private:
        static int idGenerator_;
        int id_ = idGenerator_++;
    };
}
