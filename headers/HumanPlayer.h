#pragma once

#include "Player.h"

namespace Citadel
{
    class HumanPlayer : public Player
    {
    public:
        HumanPlayer(const std::string& name);

#pragma region PURE VIRTUAL METHODS
        // Returns character picked to play
        virtual Character PickCharacter(const std::set<Character>& remainingCards) override;

        // Returns action to be taken
        virtual PlayerAction ChooseAction(const std::vector<PlayerAction>& availableActions) override;

        // Watch proposed district cards and return selected card
        virtual District WatchAndChooseDistrictCard(const std::vector<District>& districts) override;

        // Returns district card id player wants to build
        virtual std::vector<District> ChooseDistrictCardsToBuild(const size_t authorizedBuilds) override;

        // Returns character targeted by assassination or theft
        virtual Character ChooseCharacterTarget(const std::set<Character>& opponents) override;

        // Returns opponent player id, current player wants to target
        virtual int ChoosePlayerTarget(std::vector<const Player*> opponents) override;

        // Returns a pair containing player id (self district destroy is tolerated) as key and destroyed district as value
        virtual std::pair<int, District> ChoosePlayerDistrictTarget(std::vector<const Player*> players) override;

        // Returns a choice specific to Magician character
        virtual MagicianChoice MagicianDecision() override;

        // Returns discarded districts to be replaced by equivalent number of cards from district deck
        virtual std::vector<District> ChooseDistrictsCardsToSwap() override;
#pragma endregion

    private:
        std::vector<District> SelectMultipleDistricts(const std::vector<District>& container);
        void DisplayDistrictDetails(const District district, const size_t index) const;
    };
}