#include <iostream>
#include "Boardgame.h"

namespace Citadel
{
    Boardgame::Boardgame()
        : mt(rd()) // seed the random number generator once
    {
        // Setup a default game
        AddPlayer<HumanPlayer>(1);
        AddPlayer<RobotPlayer>(3);

        characterDeck_.Setup({
            Character::ASSASSIN,
            Character::THIEF,
            Character::MAGICIAN,
            Character::KING,
            Character::BISHOP,
            Character::MERCHANT,
            Character::ARCHITECT,
            Character::WARLORD,
        },
        players_.size()
        );
    }

    void Boardgame::StartBasicGame()
    {
        // Decide who's starting.
        std::uniform_int_distribution<size_t> dist(0, players_.size() - 1);
        size_t startingPlayer = dist(mt);
        
        std::cout << "[" << players_[startingPlayer]->GetName() << "] has the crown." << std::endl;


        // Setup basic stuff
        for (auto it = std::begin(players_); it != std::end(players_); ++it)
        {

        }

        size_t currentRound = 0;
        size_t currentPlayer = startingPlayer;
        size_t nextStartingPlayer = startingPlayer; // if a player plays King, he takes the crown

        do
        {
            std::cout << "[" << players_[currentPlayer]->GetName() << "] is now playing." << std::endl;
            players_[currentPlayer]->PlayTurn();

            // Next player
            ++currentPlayer;

            // Circular walking
            if (currentPlayer == players_.size())
            {
                currentPlayer = 0;
            }

            // Round ended ?
            if (currentPlayer == startingPlayer)
            {
                if (currentPlayer != nextStartingPlayer)
                {
                    std::cout << "The old king [" << players_[currentPlayer]->GetName() << "] gives the crown to the new one [" << players_[nextStartingPlayer]->GetName() << "]" << std::endl;
                    currentPlayer = nextStartingPlayer;
                    startingPlayer = nextStartingPlayer;
                }

                ++currentRound;
            }

            // debug purpose
            if (currentRound == 3)
            {
                break;
            }

        } while (IsGameEnded() == false);
    }

    void Boardgame::StartRound()
    {

    }

    bool Boardgame::IsGameEnded() const
    {
        return false;
    }
}