#include <iostream>
#include <algorithm>

#include "Logger.h"
#include "Randomness.h"
#include "HumanPlayer.h"
#include "RobotPlayer.h"
#include "Boardgame.h"

namespace Citadel
{
    void Boardgame::StartGame(const Edition edition)
    {
        SetEdition(edition);

        // Setup available characters
        characterDeck_.Setup(GetCharacterCallingOrder(edition), playerById_.size());

        // Setup available districts
        districtDeck_.Setup(GetDistricts(edition));

        // Reset ending player
        firstPlayerEndingGame = -1;

        // Decide who's starting.
        startingPlayer_ = DecideWhoStarts();

        Logger::GetInstance() << Verbosity::INFO << "[" << playerById_[startingPlayer_]->GetName() << "] has the crown." << std::endl;

        // Setup basic stuff
        // Each player earns 2 coins
        // Each player earns 4 district cards
        const size_t numberOfCards = 4;
        for (auto& pair : playerById_)
        {
            pair.second->ModifyGoldCoins(2);
            TransferDistrictCards(numberOfCards, pair.second.get());
        }

        currentPlayer_ = startingPlayer_;
        nextStartingPlayer_ = startingPlayer_; // for the moment, next king will be current king

        for (currentRound_ = 0; IsGameEnded() == false; ++currentRound_)
        {
            Logger::GetInstance() << Verbosity::DEBUG << "Round [" << currentRound_ << "]" << std::endl;
            StartRound(GetEdition());

            // TODO: remove (debug purpose)
            if (currentRound_ == 3)
            {
                break;
            }
        }

        ComputeScores();
    }

    void Boardgame::TransferDistrictCards(const size_t numberOfCards, Player* player)
    {
        if (player != nullptr)
        {
            const auto fromDeck = districtDeck_.Draw(numberOfCards);
            if (fromDeck.size() < numberOfCards)
            {
                Logger::GetInstance() << Verbosity::ERROR << "There is not enough cards in the deck. Drawn [" << fromDeck.size() << "] instead of [" << numberOfCards << "]" << std::endl;
                assert(!"District deck should have enough cards.");
            }
            auto& toHand = player->GetAvailableDistricts();
            toHand.insert(std::end(toHand), std::begin(fromDeck), std::end(fromDeck));
        }
        else
        {
            assert(!"Cannot transfer cards to nullptr player");
        }
    }

    int Boardgame::DecideWhoStarts()
    {
        if (playerById_.empty() == false)
        {
            auto random_it = std::next(std::begin(playerById_), Dice::GetRandomNumber(0, playerById_.size() - 1));
            assert(random_it->first == random_it->second->GetID()); // Logic assertion
            return random_it->second->GetID();
        }
        else
        {
            Logger::GetInstance() << Verbosity::ERROR << "There is no player to pick to start a game" << std::endl;
            assert(!"Player container is empty, cannot start a game");
        }
        return -1;
    }

    void Boardgame::StartRound(const Edition edition)
    {
        // Step One : Remove characters
        RemoveCharactersStep(edition);

        // Step Two : Choose Characters
        ChooseCharactersStep(edition);

        // Step Three: Player Turns
        PlayerTurnsStep(edition);
    }

    // Step One : Remove characters
    void Boardgame::RemoveCharactersStep(const Edition edition)
    {
        characterDeck_.RemoveCharactersStep(edition);
    }

    // Step Two : Choose Characters
    void Boardgame::ChooseCharactersStep(const Edition edition)
    {
        playerByCharacter_.clear();
        const auto& remainingCards = characterDeck_.GetRemainingCards();
        do
        {
            assert(currentPlayer_ >= 0 && currentPlayer_ < static_cast<int>(playerById_.size()));
            Logger::GetInstance() << Verbosity::INFO << "[" << playerById_[currentPlayer_]->GetName() << "] is now picking a role." << std::endl;

            const auto pickedCharacter = playerById_[currentPlayer_]->PickCharacter(remainingCards);

            // Check if role is available
            if (remainingCards.find(pickedCharacter) == remainingCards.end())
            {
                Logger::GetInstance() << Verbosity::ERROR << "Error: @" << playerById_[currentPlayer_]->GetName() << ", role is not available, try again..." << std::endl;
                continue;
            }

            // Remember next king for the next round
            if (pickedCharacter == Character::KING)
            {
                // Even murdered, currentPlayer will be the next king on next round
                nextStartingPlayer_ = currentPlayer_;
                Logger::GetInstance() << Verbosity::DEBUG << "Next king will be [" << playerById_[currentPlayer_]->GetName() << "]" << std::endl;
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
            if (currentPlayer_ == static_cast<int>(playerById_.size()))
            {
                currentPlayer_ = 0;
            }

        } while (currentPlayer_ != startingPlayer_);

        // Withdraw remaining cards (if necessary) to faceoff heap
        characterDeck_.ChooseCharactersStep();
    }

    // Step Three: Player Turns
    void Boardgame::PlayerTurnsStep(const Edition edition)
    {
        // Assassin can murder any character
        Character murderedCharacter = Character::UNINITIALIZED;

        // Thief can steal any character except Assassin
        Character stolenCharacter = Character::UNINITIALIZED;

        const auto& callingOrder = GetCharacterCallingOrder(edition);

        assert(callingOrder.empty() == false);
        for (const auto character : callingOrder)
        {
            assert(character != Character::MAX);
            assert(character != Character::UNINITIALIZED);
            Logger::GetInstance() << Verbosity::INFO << "Calling [" << GetCharacterName(character) << "]" << std::endl;
            auto it = playerByCharacter_.find(character);
            if (it == playerByCharacter_.end())
            {
                Logger::GetInstance() << Verbosity::DEBUG << "No one picked [" << GetCharacterName(character) << "]" << std::endl;
                continue;
            }

            Player* player = it->second;
            if (player == nullptr)
            {
                assert(!"player pointer should not be nullptr.");
                Logger::GetInstance() << Verbosity::ERROR << "player attached to [" << GetCharacterName(character) << "] was nullptr." << std::endl;
                continue;
            }
            Logger::GetInstance() << Verbosity::DEBUG << "[" << player->GetName() << "] is [" << GetCharacterName(character) << "]" << std::endl;

            // First check if character is murdered (assassin cannot be)
            assert(murderedCharacter != Character::ASSASSIN);
            if (character == murderedCharacter)
            {
                // Debug block
                {
                    Logger::GetInstance() << Verbosity::DEBUG << "[" << GetCharacterName(character) << "] has been murdered." << std::endl;
                    auto assassin = playerByCharacter_.find(Character::ASSASSIN);
                    auto victim = playerByCharacter_.find(character);
                    assert(assassin != playerByCharacter_.end());
                    assert(victim != playerByCharacter_.end());
                    if (assassin != playerByCharacter_.end() && victim != playerByCharacter_.end())
                    {
                        Logger::GetInstance() << Verbosity::DEBUG << "[" << victim->second->GetName() << "] has been murdered by [" << assassin->second->GetName() << "] !" << std::endl;
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
                auto thief = playerByCharacter_.find(Character::THIEF);
                auto victim = playerByCharacter_.find(character);

                {
                    // Debug block
                    Logger::GetInstance() << Verbosity::DEBUG << "[" << GetCharacterName(character) << "] has been stolen !" << std::endl;
                    assert(thief != playerByCharacter_.end());
                    assert(victim != playerByCharacter_.end());
                    if (thief != playerByCharacter_.end() && victim != playerByCharacter_.end())
                    {
                        Logger::GetInstance() << Verbosity::DEBUG << "[" << victim->second->GetName() << "] has been stolen by [" << thief->second->GetName() << "] !" << std::endl;
                    }
                }

                const auto stolenGold = victim->second->GetGoldCoins();
                thief->second->ModifyGoldCoins(stolenGold);
                victim->second->ModifyGoldCoins(-stolenGold);
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
            // There is no manual magic power (i.e. a choice to be made) for following characters
            case Character::KING:
            case Character::BISHOP:
            case Character::MERCHANT:
            case Character::ARCHITECT:
                return false;
            case Character::UNINITIALIZED:
            {
                Logger::GetInstance() << Verbosity::ERROR << "Error: CanUseMagicPower called with Character::UNINITIALIZED" << std::endl;
                assert(!"CanUseMagicPower called with Character::UNINITIALIZED");
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
            default: return; // Current character cannot earn gold.
        }

        // For each built district with a color matching current character color, earn 1 gold
        size_t earnedGoldCoins = 0;
        const auto& city = player->GetBuiltCity();
        for (const auto district : city)
        {
            if (GetDistrictColor(district) == playerColor)
            {
                ++earnedGoldCoins;
            }
        }
        player->ModifyGoldCoins(earnedGoldCoins);
    }

    void Boardgame::PlayerTurn(Player* player, Character& murderedCharacter, Character& stolenCharacter)
    {
        // Starting three steps for player turn
        PlayerTurnStep step = PlayerTurnStep::ACTION_STEP;

        // Determine if this character has special ability to be requested to the player
        bool canUseMagicPower = CanUseMagicPower(player->GetCharacter());

        while (step != PlayerTurnStep::ENDING_STEP)
        {
            PlayerAction action = PlayerAction::UNINITIALIZED;
            if (step == PlayerTurnStep::MAGIC_POWER_STEP && canUseMagicPower)
            {
                // Go straight to magic power handler
                action = PlayerAction::USE_MAGIC_POWER;
            }
            else
            {
                // TODO: hardcode these containers in static const std::vector<PlayerAction>
                std::vector<PlayerAction> availableActions;
                if (step == PlayerTurnStep::ACTION_STEP)
                {
                    availableActions.push_back(PlayerAction::TAKE_GOLD_COINS);
                    availableActions.push_back(PlayerAction::WATCH_DISTRICT_CARDS);
                }
                else if (step == PlayerTurnStep::BUILD_STEP)
                {
                    availableActions.push_back(PlayerAction::BUILD_DISTRICT_CARDS);
                }
                if (canUseMagicPower)
                {
                    availableActions.push_back(PlayerAction::USE_MAGIC_POWER);
                }

                // Ask player to choose an action
                assert(availableActions.empty() == false);
                action = player->ChooseAction(availableActions);
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
                            auto districts = districtDeck_.Draw(2);

                            // Player can watch 2 cards but pick only one.
                            auto selectedDistrict = player->WatchAndChooseDistrictCard(districts);

                            // Check player isn't cheating.
                            auto selectedDistrictIt = std::find(std::begin(districts), std::end(districts), selectedDistrict);
                            if (selectedDistrictIt == std::end(districts))
                            {
                                Logger::GetInstance() << Verbosity::ERROR << "Player must pick a card among proposed ones" << std::endl;
                                continue;
                            }

                            // Transfer the card to player
                            player->GetAvailableDistricts().push_back(selectedDistrict);

                            // Discard other cards
                            districts.erase(selectedDistrictIt); // Remove chosen card
                            districtDeck_.Discard(districts);    // Add the rest
                        }

                        if (player->GetCharacter() == Character::ARCHITECT)
                        {
                            // Architect gains 2 card just after taking an action.
                            TransferDistrictCards(2, player);
                        }
                        else if (player->GetCharacter() == Character::MERCHANT)
                        {
                            // Merchant gains 1 gold coin just after taking an action.
                            player->ModifyGoldCoins(1);
                        }

                        step = PlayerTurnStep::BUILD_STEP;
                    }
                    else
                    {
                        if (action == PlayerAction::TAKE_GOLD_COINS)
                        {
                            Logger::GetInstance() << Verbosity::ERROR << "Player [" << player->GetName() << "] cannot take gold coins at this step" << std::endl;
                        }
                        else if (action == PlayerAction::WATCH_DISTRICT_CARDS)
                        {
                            Logger::GetInstance() << Verbosity::ERROR << "Player [" << player->GetName() << "] cannot watch two cards at this step" << std::endl;
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
                        auto districtCards = player->ChooseDistrictCardsToBuild(authorizedBuilds);

                        if (districtCards.size() > authorizedBuilds)
                        {
                            Logger::GetInstance() << Verbosity::ERROR << "Player [" << player->GetName() << "] is not able to build [" << districtCards.size() << "] districts." << std::endl;
                            continue;
                        }

                        // Iterate over chosen districts by player
                        int goldCost = 0;
                        for (const auto district : districtCards)
                        {
                            // Price each district to get the sum
                            goldCost += GetDistrictCost(district);
                        }

                        // Check if player has enough gold coins
                        if (goldCost > player->GetGoldCoins())
                        {
                            Logger::GetInstance() << Verbosity::ERROR << "Player [" << player->GetName() << "] tried to build but doesn't have enough gold coins." << std::endl;
                            continue;
                        }

                        // Finally build the district
                        player->BuildDistrict(districtCards);

                        // Record first player ending game to grant bonus points
                        if (player->GetBuiltCitySize() >= numberOfDistrictsToWin_ && firstPlayerEndingGame == -1)
                        {
                            Logger::GetInstance() << Verbosity::DEBUG << "[" << player->GetName() << "] is first player to end it's city (" << numberOfDistrictsToWin_ << ") districts built." << std::endl;
                            firstPlayerEndingGame = player->GetID();
                        }

                        // Proceed to next step
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
                        Logger::GetInstance() << Verbosity::ERROR << "Player [" << player->GetName() << "] cannot build at this step." << std::endl;
                    }
                    continue;
                }

                case PlayerAction::USE_MAGIC_POWER:
                {
                    if (canUseMagicPower)
                    {
                        const auto magicPowerConsumed = UseMagicPower(player, murderedCharacter, stolenCharacter);
                        canUseMagicPower = !magicPowerConsumed;
                        if (magicPowerConsumed && step == PlayerTurnStep::MAGIC_POWER_STEP)
                        {
                            step = PlayerTurnStep::ENDING_STEP;
                        }
                    }
                    else
                    {
                        Logger::GetInstance() << Verbosity::ERROR << "Player [" << player->GetName() << "] cannot use its magic power." << std::endl;
                    }
                    break;
                }

                default:
                {
                    Logger::GetInstance() << Verbosity::ERROR << "Player [" << player->GetName() << "] must pick a choice. Returned [" << static_cast<int>(action) << "]" << std::endl;
                }
            }
        }
    }

    // Boolean return: does magic power has been consumed ?
    bool Boardgame::UseMagicPower(Player* player, Character& murderedCharacter, Character& stolenCharacter)
    {
        const auto character = player->GetCharacter();
        switch (character)
        {
            case Character::ASSASSIN:
            {
                return AssassinMagicPower(player, murderedCharacter);
            }
            case Character::THIEF:
            {
                return ThiefMagicPower(player, stolenCharacter);
            }
            case Character::MAGICIAN:
            {
                return MagicianMagicPower(player);
            }
            case Character::WARLORD:
            {
                return WarlordMagicPower(player);
            }
            default:
            {
                Logger::GetInstance() << Verbosity::ERROR << "No magic power for [" << GetCharacterName(player->GetCharacter()) << "]" << std::endl;
            }
        }
        return true;
    }

    bool Boardgame::AskCharacterTarget(Player* player, Character& victim)
    {
        auto possibleVictims = characterDeck_.PossibleOpponentsCharacters(player->GetCharacter());
        victim = player->ChooseCharacterTarget(possibleVictims);
        if (possibleVictims.find(victim) == std::end(possibleVictims))
        {
            Logger::GetInstance() << Verbosity::ERROR << "Player [" << player->GetName() << "] choosed [" << GetCharacterName(victim) << "] but it's impossible." << std::endl;
            return false;
        }
        return true;
    }

    bool Boardgame::AssassinMagicPower(Player* player, Character& murderedCharacter)
    {
        Character victim = Character::UNINITIALIZED;

        if (AskCharacterTarget(player, victim))
        {
            murderedCharacter = victim;
            return true;
        }

        return false;
    }

    bool Boardgame::ThiefMagicPower(Player* player, Character& stolenCharacter)
    {
        Character victim = Character::UNINITIALIZED;

        if (AskCharacterTarget(player, victim))
        {
            stolenCharacter = victim;
            return true;
        }

        return false;
    }

    bool Boardgame::MagicianMagicPower(Player* player)
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
                return MagicianExchangeFromPlayer(player);
            }
            case MagicianChoice::EXCHANGE_FROM_DISTRICT_DECK:
            {
                return MagicianExchangeFromDistrictDeck(player);
            }
            case MagicianChoice::DO_NOTHING:
            {
                // Player has chosen to not use his magic power
                break;
            }
            default:
            {
                Logger::GetInstance() << Verbosity::ERROR << "Magician [" << player->GetName() << "] choice is not correct: [" << static_cast<int>(magicianChoice) << "]" << std::endl;
                return false;
            }
        }

        return true;
    }

    bool Boardgame::MagicianExchangeFromPlayer(Player* player)
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

        if (victimID == player->GetID())
        {
            Logger::GetInstance() << Verbosity::ERROR << "Player [" << player->GetName() << "] cannot self swap available districts" << std::endl;
            return false;
        }

        auto playerIdPairIt = playerById_.find(victimID);
        if (playerIdPairIt == playerById_.end())
        {
            Logger::GetInstance() << Verbosity::ERROR << "Unable to find [" << victimID << "] player ID. Retry." << std::endl;
            return false;
        }

        // Swap cards in hand
        std::swap(player->GetAvailableDistricts(), playerIdPairIt->second->GetAvailableDistricts());

        return true;
    }

    bool Boardgame::MagicianExchangeFromDistrictDeck(Player* player)
    {
        auto& cardsInHand = player->GetAvailableDistricts();
        if (cardsInHand.empty())
        {
            Logger::GetInstance() << Verbosity::ERROR << "Player [" << player->GetName() << "] cannot swap with districts deck, no district to swap !" << std::endl;
            return false;
        }

        if (districtDeck_.GetPileOfCardSize() < cardsInHand.size())
        {
            Logger::GetInstance() << Verbosity::ERROR << "Player [" << player->GetName() << "] cannot swap with districts deck, there is not enough districts in deck to swap" << std::endl;
            return false;
        }

        auto districtsToDiscard = player->ChooseDistrictsCardsToSwap();
        if (districtsToDiscard.empty())
        {
            Logger::GetInstance() << Verbosity::ERROR << "Player [" << player->GetName() << "] returned no districts to exchange with District deck" << std::endl;
            return false;
        }

        auto end = districtsToDiscard.end();
        for (auto it = districtsToDiscard.begin(); it != end; ++it)
        {
            auto pos = std::find(cardsInHand.begin(), cardsInHand.end(), *it);
            if (pos != cardsInHand.end())
            {
                // Remove card from hand
                cardsInHand.erase(pos);
            }
            else
            {
                Logger::GetInstance() << Verbosity::ERROR << "Player [" << player->GetName() << "] chose district [" << GetDistrictName(*it) << "] he does not own" << std::endl;
                // Insert removed cards to rollback
                cardsInHand.insert(cardsInHand.end(), districtsToDiscard.begin(), it);
                return false;
            }
        }

        districtDeck_.Discard(districtsToDiscard);
        auto drawnCards = districtDeck_.Draw(districtsToDiscard.size());
        cardsInHand.insert(cardsInHand.end(), drawnCards.begin(), drawnCards.end());

        return true;
    }

    bool Boardgame::WarlordMagicPower(Player* player)
    {
        std::vector<const Player*> players;
        for (const auto& pair : playerById_)
        {
            players.push_back(pair.second.get());
        }

        auto pair = player->ChoosePlayerDistrictTarget(players);

        // Find player
        auto victimIt = playerById_.find(pair.first);
        if (victimIt == playerById_.end())
        {
            Logger::GetInstance() << Verbosity::ERROR << "Unable to find player id [" << pair.first << "]" << std::endl;
            return false;
        }

        // Warlord cannot destroy Bishop districts
        if (victimIt->second->GetCharacter() == Character::BISHOP)
        {
            Logger::GetInstance() << Verbosity::ERROR << "Player [" << player->GetName() << "] cannot destroy Bishop district" << std::endl;
            return false;
        }

        // Once a city is completed, one district cannot be destroyed within.
        if (victimIt->second->GetBuiltCitySize() >= numberOfDistrictsToWin_)
        {
            Logger::GetInstance() << Verbosity::ERROR << "Player [" << player->GetName() << "] cannot destroy a district in a finished Citadel" << std::endl;
            return false;
        }

        if (pair.second == District::UNINITIALIZED)
        {
            Logger::GetInstance() << Verbosity::ERROR << "Player [" << player->GetName() << "] cannot destroy UNINITIALIZED district" << std::endl;
            assert(!"Cannot destroy UNINITIALIZED district.");
            return false;
        }

        if (GetDistrictCost(pair.second) >= 1)
        {
            // Destroy cost is district cost minus one.
            if (GetDistrictCost(pair.second) - 1 <= player->GetGoldCoins())
            {
                if (victimIt->second->DestroyDistrict(pair.second) == false)
                {
                    Logger::GetInstance() << Verbosity::ERROR << "Player [" << pair.first << "] does not have [" << GetDistrictName(pair.second) << "]" << std::endl;
                    return false;
                }
                Logger::GetInstance() << Verbosity::DEBUG << "player [" << player->GetName() << "] destroyed [" << GetDistrictName(pair.second) << "] owned by [" << victimIt->second->GetName() << "]" << std::endl;
            }
            else
            {
                Logger::GetInstance() << Verbosity::ERROR << "Player [" << pair.first << "] has not enough gold to destroy this district" << std::endl;
                return false;
            }
        }
        else
        {
            Logger::GetInstance() << Verbosity::ERROR << "District [" << GetDistrictName(pair.second) << "] has a cost of [" << GetDistrictCost(pair.second) << "] gold coins." << std::endl;
            return false;
        }

        return true;
    }

    bool Boardgame::IsGameEnded() const
    {
        for (const auto& pair : playerById_)
        {
            if (pair.second->GetBuiltCity().size() >= numberOfDistrictsToWin_)
            {
                return true;
            }
        }
        return false;
    }

    void Boardgame::ComputeScores()
    {
        // Key: player id
        // Value: score
        std::unordered_map<int, int> scores;
        std::set<Color> colorBonus;

        for (const auto& pair : playerById_)
        {
            colorBonus.clear();
            scores.insert(std::make_pair(pair.first, 0));
            Player* player = pair.second.get();

            if (player == nullptr)
            {
                continue;
            }

            assert(player->GetID() == pair.first);

            // Score rules
            // 1. A player receives a number of points equal to the total combined gold cost of all the district cards in his city at the end of the game.
            // 2. If a player has at least one district in each of the five colors, he receives three points.
            // 3. If a player was the first player to build eight districts, he receives four points.
            // 4. All other players who have managed to build eight districts at the end of the game receive two points.

            int score = 0;

            const auto& city = player->GetBuiltCity();
            for (auto district : city)
            {
                colorBonus.insert(GetDistrictColor(district)); // Rule 2
                score += GetDistrictCost(district); // Rule 1
            }

            // Rule 2
            if (colorBonus.size() == GetColorNumber())
            {
                score += 3;
            }

            // Rule 3
            if (player->GetID() == firstPlayerEndingGame)
            {
                score += 4;
            }
            // Rule 4
            else if (city.size() >= numberOfDistrictsToWin_)
            {
                score += 2;
            }

            scores[player->GetID()] = score;

            Logger::GetInstance() << Verbosity::DEBUG << "Player [" << player->GetName() << "] has [" << score << "] points." << std::endl;
        }
    }
}