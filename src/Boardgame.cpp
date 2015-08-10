#include <iostream>

#include "Randomness.h"
#include "HumanPlayer.h"
#include "RobotPlayer.h"
#include "Boardgame.h"

namespace
{
    enum class PlayerTurnStep
    {
        ACTION_STEP,
        BUILD_STEP,
        MAGIC_POWER_STEP,
        ENDING_STEP
    };

#pragma region DEFAULT CHARACTERS
    static const std::vector<Character> defaultCharacters
    {
        Character::ASSASSIN,
        Character::THIEF,
        Character::MAGICIAN,
        Character::KING,
        Character::BISHOP,
        Character::MERCHANT,
        Character::ARCHITECT,
        Character::WARLORD,
    };
#pragma endregion

#pragma region DEFAULT DISTRICTS
    static const std::vector<District> defaultDistricts
    {
#pragma region RED DISTRICTS
        District::WATCHTOWER,
        District::PRISON,
        District::BATTLEFIELD,
        District::FORTRESS,
#pragma endregion
#pragma region YELLOW DISTRICTS
        District::MANOR,
        District::CASTLE,
        District::PALACE,
#pragma endregion
#pragma region GREEN DISTRICTS
        District::TAVERN,
        District::MARKET,
        District::TRADING_POST,
        District::DOCKS,
        District::HARBOR,
        District::TOWN_HALL,
#pragma endregion
#pragma region BLUE DISTRICTS
        District::TEMPLE,
        District::CHURCH,
        District::MONASTARY,
        District::CATHEDRAL,
#pragma endregion
    };
#pragma endregion
}

namespace Citadel
{
    Boardgame::Boardgame()
    {
        // Setup a basic game

        //AddPlayer<HumanPlayer>(1);
        AddPlayer<RobotPlayer>(4);

        // Setup available characters
        characterDeck_.Setup(defaultCharacters, playerById_.size());

        // Setup available districts
        districtDeck_.Setup(defaultDistricts);
    }

    void Boardgame::StartBasicGame()
    {
        // Decide who's starting.
        startingPlayer_ = DecideWhoStarts();

        std::cout << "[" << playerById_[startingPlayer_]->GetName() << "] has the crown." << std::endl;

        // Setup basic stuff
        // Each player earns 2 coins
        // Each player earns 4 district cards
        for (auto it = std::begin(playerById_); it != std::end(playerById_); ++it)
        {
            it->second->ModifyGoldCoins(2);
        }

        currentPlayer_ = startingPlayer_;
        nextStartingPlayer_ = startingPlayer_; // for the moment, next king will be current king

        for (currentRound_ = 0; IsGameEnded() == false; ++currentRound_)
        {
            StartRound();

            // Debug purpose
            if (currentRound_ == 3)
            {
                break;
            }
        }
    }

    int Boardgame::DecideWhoStarts()
    {
        assert(playerById_.size() > 0);
        if (playerById_.size() > 0)
        {
            auto random_it = std::next(std::begin(playerById_), Dice::GetRandomNumber(0, playerById_.size() - 1));
            assert(random_it->first == random_it->second->GetID());
            return random_it->second->GetID();
        }
        return -1;
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
        playerByCharacter_.clear();
        const auto& remainingCards = characterDeck_.GetRemainingCards();
        do
        {
            assert(currentPlayer_ >= 0 && currentPlayer_ < playerById_.size());
            std::cout << "[" << playerById_[currentPlayer_]->GetName() << "] is now picking a role." << std::endl;

            const auto pickedCharacter = playerById_[currentPlayer_]->PickCharacter(remainingCards);

            // Check if role is available
            if (remainingCards.find(pickedCharacter) == remainingCards.end())
            {
                std::cerr << "Error: @" << playerById_[currentPlayer_]->GetName() << ", role is not available, try again..." << std::endl;
                continue;
            }

            // Remember next king for the next round
            if (pickedCharacter == Character::KING)
            {
                // Note even murdered, currentPlayer will be the next king on next round
                nextStartingPlayer_ = currentPlayer_;
                std::cout << "Debug: Next king will be [" << playerById_[currentPlayer_]->GetName() << "]" << std::endl;
            }

            // Confirm picked character
            playerById_[currentPlayer_]->SetCharacter(pickedCharacter);

            // Update player by role index (speed-up calling roles on next step)
            playerByCharacter_[pickedCharacter] = playerById_[currentPlayer_].get();

            // Remove character from remainingCards
            characterDeck_.RemoveCard(pickedCharacter);

            // Next player
            ++currentPlayer_;

            // Circular walking
            if (currentPlayer_ == playerById_.size())
            {
                currentPlayer_ = 0;
            }

        } while (currentPlayer_ != startingPlayer_);

        // Withdraw remaining cards (if necessary) to faceoff heap
        characterDeck_.ChooseCharactersStep();
    }

    // Step Three: Player Turns
    void Boardgame::PlayerTurnsStep()
    {
        // TODO: move calling order to boardgame attribute + manage new roles such as Witch
        static const std::vector<Character> callingOrder
        {
            Character::ASSASSIN,
            Character::THIEF,
            Character::MAGICIAN,
            Character::KING,
            Character::BISHOP,
            Character::MERCHANT,
            Character::ARCHITECT,
            Character::WARLORD,
        };

        // Assassin can murder any character
        Character murderedCharacter = Character::UNINITIALIZED;

        // Thief can steal any character except Assassin
        Character stolenCharacter = Character::UNINITIALIZED;

        for (const auto character : callingOrder)
        {
            assert(character != Character::MAX);
            assert(character != Character::UNINITIALIZED);
            std::cout << "Calling [" << GetCharacterName(character) << "]" << std::endl;
            auto it = playerByCharacter_.find(character);
            if (it == playerByCharacter_.end())
            {
                std::cout << "Debug: No one picked [" << GetCharacterName(character) << "]" << std::endl;
                continue;
            }

            Player* player = it->second;
            if (player == nullptr)
            {
                assert(!"player pointer should not be nullptr.");
                std::cerr << "Error: player attached to [" << GetCharacterName(character) << "] was nullptr." << std::endl;
                continue;
            }
            std::cout << "Debug: [" << player->GetName() << "] is [" << GetCharacterName(character) << "]" << std::endl;

            // First check if character is murdered (assassin cannot be)
            assert(murderedCharacter != Character::ASSASSIN);
            if (character == murderedCharacter)
            {
                // Debug block
                {
                    std::cout << "Debug: [" << GetCharacterName(character) << "] has been murdered." << std::endl;
                    auto assassin = playerByCharacter_.find(Character::ASSASSIN);
                    auto victim = playerByCharacter_.find(character);
                    assert(assassin != playerByCharacter_.end());
                    assert(victim != playerByCharacter_.end());
                    if (assassin != playerByCharacter_.end() && victim != playerByCharacter_.end())
                    {
                        std::cout << "Debug: [" << victim->second->GetName() << "] has been murdered by [" << assassin->second->GetName() << "] !" << std::endl;
                    }
                }
                // Current player skip it's turn
                continue;
            }

            // Then check if character is stolen
            assert(stolenCharacter != Character::ASSASSIN);
            assert(stolenCharacter != Character::THIEF);
            if (character == stolenCharacter)
            {
                {
                    // Debug block
                    std::cout << "Debug: [" << GetCharacterName(character) << "] has been stolen !" << std::endl;
                    auto thief = playerByCharacter_.find(Character::THIEF);
                    auto victim = playerByCharacter_.find(character);
                    assert(thief != playerByCharacter_.end());
                    assert(victim != playerByCharacter_.end());
                    if (thief != playerByCharacter_.end() && victim != playerByCharacter_.end())
                    {
                        std::cout << "Debug: [" << victim->second->GetName() << "] has been stolen by [" << thief->second->GetName() << "] !" << std::endl;
                    }
                }

                // TODO: implement STEALING
            }

            // 1) Earn gold from district cards
            EarnGoldFromDistricts(character, player);

            // Note Player uses his magic power whenever he wants but will be requested after building
            // 2) Take an action: player earns 2 gold coins OR watch 2 district cards, pick one, place the other to district cards heap bottom
            // 3) Build a district card from his hand
            // 4) Use magic power if any
            PlayerTurn(player, murderedCharacter, stolenCharacter);
        }
    }

    bool Boardgame::CanUseMagicPower(const Character character) const
    {
        switch (character)
        {
            // There is no manual magic power (i.e. a choice to be made)
            case Character::KING:
            case Character::BISHOP:
            case Character::MERCHANT:
                return false;
            case Character::UNINITIALIZED:
            {
                assert(!"Cannot call CanUseMagicPower on Character::UNINITIALIZED");
                std::cerr << "Error: CanUseMagicPower called with Character::UNINITIALIZED" << std::endl;
                return false;
            }
            default:;
        }
        return true;
    }

    void Boardgame::EarnGoldFromDistricts(const Character character, Player* player)
    {
        Color playerColor = Color::UNINITIALIZED;

        switch (character)
        {
            // Only these characters earn gold from their respective district
            case Character::KING: playerColor = Color::YELLOW; break;
            case Character::BISHOP: playerColor = Color::BLUE; break;
            case Character::MERCHANT: playerColor = Color::GREEN; break;
            case Character::WARLORD: playerColor = Color::RED; break;
            default:;
        }

        // TODO: count colored district and earn equivalent gold coins
    }

    void Boardgame::PlayerTurn(Player* player, Character& murderedCharacter, Character& stolenCharacter)
    {
        // Starting three steps for player turn
        // TODO: change ENUM CLASS name...
        PlayerTurnStep step = PlayerTurnStep::ACTION_STEP;

        // Determine if this character has special ability to be requested to the player
        bool canUseMagicPower = CanUseMagicPower(player->GetCharacter());

        while (step != PlayerTurnStep::ENDING_STEP)
        {
            PlayerAction action = PlayerAction::UNITIALIZED;
            if (step == PlayerTurnStep::MAGIC_POWER_STEP && canUseMagicPower)
            {
                // Go straight to magic power handler
                action = PlayerAction::USE_MAGIC_POWER;
            }
            else
            {
                // Ask player to choose an action (gold coins or district cards)
                action = player->ChooseAction();
            }

            switch (action)
            {
                case PlayerAction::TAKE_GOLD_COINS:
                case PlayerAction::WATCH_DISTRICT_CARDS:
                {
                    if (step == PlayerTurnStep::ACTION_STEP)
                    {
                        if (action == PlayerAction::TAKE_GOLD_COINS)
                        {
                            player->ModifyGoldCoins(2);
                        }
                        else if (action == PlayerAction::WATCH_DISTRICT_CARDS)
                        {
                            // Player can watch 2 cards but pick only one.
                            //int cardID = player->PickDistrictCard();
                        }
                        if (player->GetCharacter() == Character::ARCHITECT)
                        {
                            // Architect gains 2 card just after taking an action.
                            // TODO: implement gaining 2 district cards
                        }
                        step = PlayerTurnStep::BUILD_STEP;
                    }
                    else
                    {
                        if (action == PlayerAction::TAKE_GOLD_COINS)
                        {
                            std::cerr << "Error: You cannot take gold coins at this step" << std::endl;
                        }
                        else if (action == PlayerAction::WATCH_DISTRICT_CARDS)
                        {
                            std::cerr << "Error: You cannot watch two cards at this step" << std::endl;
                        }
                        else
                        {
                            assert(!"LOGIC FAILURE");
                        }
                    }
                    continue;
                }

                case PlayerAction::BUILD_DISTRICT_CARDS:
                {
                    if (step == PlayerTurnStep::BUILD_STEP)
                    {
                        // Ask player for building
                        // Note Architect can build up to three district cards
                        const size_t authorizedBuilds = player->GetCharacter() == Character::ARCHITECT ? 3 : 1;
                        auto districts = player->ChooseDistrictCardsToBuild(authorizedBuilds);

                        if (canUseMagicPower)
                        {
                            step = PlayerTurnStep::MAGIC_POWER_STEP;
                        }
                        else
                        {
                            step = PlayerTurnStep::ENDING_STEP;
                        }
                    }
                    else
                    {
                        std::cerr << "You cannot build at this step" << std::endl;
                    }
                    continue;
                }

                case PlayerAction::USE_MAGIC_POWER:
                {
                    if (canUseMagicPower)
                    {
                        const auto magicPowerConsumed = UseMagicPower(player, murderedCharacter, stolenCharacter);
                        canUseMagicPower = false;
                    }
                    else
                    {
                        std::cerr << "You cannot use your magic power" << std::endl;
                    }
                    break;
                }

                default:
                {
                    std::cerr << "Player [" << player->GetName() << "] must pick a choice. Returned [" << static_cast<int>(action) << "]" << std::endl;
                }
            }
        }
    }

    // Boolean return: does magic power has been consumed ?
    bool Boardgame::UseMagicPower(Player* player, Character& murderedCharacter, Character& stolenCharacter)
    {
        switch (player->GetCharacter())
        {
            case Character::ASSASSIN:
            {
                const Character victim = player->ChooseCharacterTarget();

                // Ensure player is not trying to commit suicide
                if (victim == player->GetCharacter())
                {
                    std::cerr << "Suicide is not allowed..." << std::endl;
                    return false;
                }

                // Ensure player is not trying to murder a character which is not used
                const auto& faceupCards = characterDeck_.GetFaceupCards();
                if (faceupCards.find(victim) != faceupCards.end())
                {
                    std::cerr << "There is no point to assassinate faceup character..." << std::endl;
                    return false;
                }

                murderedCharacter = victim;
                break;
            }
            case Character::THIEF:
            {
                const Character victim = player->ChooseCharacterTarget();

                // Ensure player is not trying to rob himself
                if (victim == player->GetCharacter())
                {
                    std::cerr << "Self stealing is not allowed..." << std::endl;
                    return false;
                }

                // Ensure player is not trying to steal a character which is not used
                const auto& faceupCards = characterDeck_.GetFaceupCards();
                if (faceupCards.find(victim) != faceupCards.end())
                {
                    std::cerr << "There is no point to assassinate faceup character..." << std::endl;
                    return false;
                }

                stolenCharacter = victim;
                break;
            }
            case Character::MAGICIAN:
            {
                // Three possibilities:
                // 1) Exchange your entire hand of cards(not the cards in your city) with the hand of another player
                // 2)  Place any number of cards from your hand facedown at the bottom of the District Deck, and
                //     then draw an equal number of cards from the top of the District Deck
                // 3) Do nothing
                const MagicianChoice magicianChoice = player->MagicianDecision();
                switch (magicianChoice)
                {
                    case MagicianChoice::EXCHANGE_FROM_PLAYER:
                    {
                        // Build a vector of readonly players
                        std::vector<const Player*> opponents;
                        for (const auto& pair : playerById_)
                        {
                            if (pair.first != player->GetID())
                            {
                                opponents.push_back(pair.second.get());
                            }
                        }

                        const int victimID = player->ChoosePlayerTarget(opponents);
                        // TODO: implement card swap between players

                        if (victimID == player->GetID())
                        {
                            std::cerr << "Cannot self swap card" << std::endl;
                            return false;
                        }

                        auto playerIdPairIt = playerById_.find(victimID);
                        if (playerIdPairIt == playerById_.end())
                        {
                            std::cerr << "Unable to find [" << victimID << "] player ID. Retry." << std::endl;
                            return false;
                        }

                        // Swap cards in hand
                        std::swap(player->GetCardsInHand(), playerIdPairIt->second->GetCardsInHand());

                        break;
                    }
                    case MagicianChoice::EXCHANGE_FROM_DISTRICT_DECK:
                    {
                        // TODO: implement district card swap with district deck
                        break;
                    }
                    case MagicianChoice::DO_NOTHING:
                    {
                        break;
                    }
                    default:
                    {
                        std::cerr << "Magician choice is not correct: [" << static_cast<int>(magicianChoice) << "]" << std::endl;
                    }
                }
                break;
            }
            default:
            {
                std::cerr << "No magic power for [" << GetCharacterName(player->GetCharacter()) << "]" << std::endl;
            }
        }
        return true;
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