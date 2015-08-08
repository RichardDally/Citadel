#include <iostream>

#include "Randomness.h"
#include "HumanPlayer.h"
#include "RobotPlayer.h"
#include "Boardgame.h"

namespace Citadel
{
    Boardgame::Boardgame()
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
        assert(players_.size() > 0);
        startingPlayer_ = Dice::GetRandomNumber(0, players_.size() - 1);
        
        std::cout << "[" << players_[startingPlayer_]->GetName() << "] has the crown." << std::endl;

        // Setup basic stuff
        // Each player earns 2 coins
        // Each player earns 4 district cards
        for (auto it = std::begin(players_); it != std::end(players_); ++it)
        {
            (*it)->ModifyGoldCoins(2);
        }

        currentPlayer_ = startingPlayer_;
        nextStartingPlayer_ = startingPlayer_; // for the moment, next king will be current king

        for (currentRound_ = 0; IsGameEnded() == false; ++currentRound_)
        {
            StartRound();

            // debug purpose
            if (currentRound_ == 3)
            {
                break;
            }
        }
    }

    void Boardgame::StartRound()
    {
        // Step One : Remove characters
        RemoveCharactersStep();

        // Step Two : Choose Characters
        ChooseCharactersStep();

        // Step Three: Player Turns
        PlayerTurnsStep();

        // Step Four : End of Round
        EndOfTurnStep();
    }

    // Step One : Remove characters
    void Boardgame::RemoveCharactersStep()
    {
        characterDeck_.RemoveCharactersStep();
    }

    // Step Two : Choose Characters
    void Boardgame::ChooseCharactersStep()
    {
        auto remainingCards = characterDeck_.GetRemainingCards();

        do
        {
            std::cout << "[" << players_[currentPlayer_]->GetName() << "] is now picking a role." << std::endl;

            const auto pickedCharacter = players_[currentPlayer_]->PickRoleStep(remainingCards);
            
            // Check if role is available
            if (remainingCards.find(pickedCharacter) == remainingCards.end())
            {
                std::cerr << "Role is not available, try again..." << std::endl;
                continue;
            }

            // Confirm picked character
            players_[currentPlayer_]->SetCharacter(pickedCharacter);

            // Remove character from remainingCards
            characterDeck_.RemoveCard(pickedCharacter);

            // Next player
            ++currentPlayer_;

        } while (currentPlayer_ == startingPlayer_);

        // Circular walking
        if (currentPlayer_ == players_.size())
        {
            currentPlayer_ = 0;
        }

        // Round ended ?
        /*
        if (currentPlayer_ == startingPlayer_)
        {
            if (currentPlayer_ != nextStartingPlayer_)
            {
                std::cout << "The old king [" << players_[currentPlayer_]->GetName() << "] gives the crown to the new one [" << players_[nextStartingPlayer_]->GetName() << "]" << std::endl;
                currentPlayer_ = nextStartingPlayer_;
                startingPlayer_ = nextStartingPlayer_;
            }

            ++currentRound_;
        }
        */
    }

    // Step Three: Player Turns
    void Boardgame::PlayerTurnsStep()
    {
        // Each player play his role when character is called
        // Call every role one by one
        // 1) ASSASSIN
        // 2) THIEF
        // 3) MAGICIAN
        // 4) KING
        // 5) BISHOP
        // 6) MERCHANT
        // 7) ARCHITECT
        // 8) WARLORD
    }

    // Step Four : End of Round
    void Boardgame::EndOfTurnStep()
    {
    }

    bool Boardgame::IsGameEnded() const
    {
        return false;
    }
}