#pragma once

#include "Player.h"

namespace Citadel
{
    class RobotPlayer : public Player
    {
    public:
        RobotPlayer();
        RobotPlayer(const std::string& name);

        // Returns character picked to play
        virtual Character PickCharacter(const std::unordered_set<Character>& remainingCards) override;

        // Returns action to be taken
        virtual PlayerAction ChooseAction() override;

        // Returns district card id player wants to build
        virtual std::vector<int> ChooseDistrictCardsToBuild(const size_t authorizedBuilds) override;

        // Returns character targeted by assassination or theft
        virtual Character ChooseCharacterTarget() override;

        // Returns opponent player id, current player wants to target
        virtual int ChoosePlayerTarget(std::vector<const Player*> opponents) override;

        // Returns a choice specific to Magician character
        virtual MagicianChoice MagicianDecision() override;
    };
}