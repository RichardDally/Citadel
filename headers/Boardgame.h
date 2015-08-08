#pragma once

#include <random>
#include <vector>
#include <memory>

#include "Player.h"
#include "CharacterDeck.h"

namespace Citadel
{
    class Boardgame
    {
    public:
        Boardgame();
        // TODO: Add ctor to customize game
        // TODO: Add reset function

        void StartBasicGame();

    private:
        void StartRound();
        bool IsGameEnded() const;

        template <typename PlayerType>
        void AddPlayer(const size_t n)
        {
            static_assert(std::is_same<PlayerType, HumanPlayer>::value || std::is_same<PlayerType, RobotPlayer>::value,
                "PlayerType must be HumanPlayer or RobotPlayer");
            for (size_t i = 0; i < n; ++i)
            {
                players_.emplace_back(new PlayerType);
            }
        }

        std::random_device rd;
        std::mt19937 mt;

        CharacterDeck characterDeck_;
        std::vector<std::unique_ptr<Player>> players_;
    };
}