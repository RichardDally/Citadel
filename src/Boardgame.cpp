#include <iostream>
#include <algorithm>
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
        firstPlayerEndingGame_ = -1;

        // Decide who's starting.
        startingPlayer_ = DecideWhoStarts();

        spdlog::info("[{}] has the crown.", playerById_[startingPlayer_]->GetName());

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
            spdlog::debug("Round [{}]", currentRound_ );
            StartRound(GetEdition());
        }

        ComputeScores();
    }

    void Boardgame::TransferDistrictCards(const size_t numberOfCards, Player* player)
    {
        if (player != nullptr)
        {
            const auto fromDeck = districtDeck_.GetDistricts(DistrictDeckAction::DRAW, numberOfCards);
            if (fromDeck.size() < numberOfCards)
            {
                spdlog::warn("There is not enough cards in the deck. Drawn [{}] instead of [{}]", fromDeck.size(), numberOfCards);
            }
            auto& toHand = player->GetAvailableDistricts();
            toHand.insert(std::end(toHand), std::begin(fromDeck), std::end(fromDeck));
        }
        else
        {
            spdlog::critical("Player is nullptr in TransferDistrictCards");
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
            spdlog::error("There is no player to pick to start a game");
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
        const auto& remainingCharacters = characterDeck_.GetRemainingCharacters();
        do
        {
            assert(currentPlayer_ >= 0 && currentPlayer_ < static_cast<int>(playerById_.size()));
            spdlog::info("[{}] is now picking a role.", playerById_[currentPlayer_]->GetName());

            /*
            spdlog::debug("Available roles to choose [";
            const size_t remaining = remainingCharacters.size();
            size_t index = 0;
            for (const auto& character : remainingCharacters)
            {
                Logger::GetInstance() << GetCharacterName(character);
                if (++index != remaining)
                {
                    Logger::GetInstance() << " ";
                }
            }
            Logger::GetInstance() << "]" << std::endl;
            */

            const auto pickedCharacter = playerById_[currentPlayer_]->PickCharacter(remainingCharacters, characterDeck_.GetFaceupCharacters());

            // Check if role is available
            if (remainingCharacters.find(pickedCharacter) == remainingCharacters.end())
            {
                spdlog::error("{}, role is not available, try again...", playerById_[currentPlayer_]->GetName());
                continue;
            }

            // Remember next king for the next round
            if (pickedCharacter == Character::KING)
            {
                // Even murdered, currentPlayer will be the next king on next round
                nextStartingPlayer_ = currentPlayer_;
                spdlog::debug("Next king will be [{}]", playerById_[currentPlayer_]->GetName());
            }

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

            if (playerById_.size() == 2)
            {
                if (remainingCharacters.size() == 5 || remainingCharacters.size() == 3)
                {
                    characterDeck_.WithdrawCharacterToFaceOff();
                }
            }
        }
        while (remainingCharacters.size() != 1);

        // Withdraw remaining cards (if necessary) to faceoff heap
        characterDeck_.ChooseCharactersStep();
    }

    // Step Three: Player Turns
    void Boardgame::PlayerTurnsStep(const Edition edition)
    {
        victims.clear();
        const auto& callingOrder = GetCharacterCallingOrder(edition);
        assert(callingOrder.empty() == false);
        for (const auto character : callingOrder)
        {
            assert(character != Character::MAX);
            assert(character != Character::UNINITIALIZED);
            spdlog::info("Calling [{}]", GetCharacterName(character));
            auto it = playerByCharacter_.find(character);
            if (it == playerByCharacter_.end())
            {
                spdlog::debug("No one picked [{}]", GetCharacterName(character));
                continue;
            }

            Player* player = it->second;
            if (player == nullptr)
            {
                spdlog::critical("Player attached to [{}] was nullptr.", GetCharacterName(character));
                assert(!"player pointer should not be nullptr.");
                continue;
            }
            player->SetCharacter(character);
            spdlog::debug("[{}] is [{}]", player->GetName(), GetCharacterName(character));

            const auto victimsIt = victims.find(character);
            if (victimsIt != std::end(victims))
            {
                const auto victim = victimsIt->first;
                const auto offenser = victimsIt->second;
                assert(victim != Character::ASSASSIN);
                if (offenser == Character::ASSASSIN)
                {
                    // Debug block
                    {
                        spdlog::debug("[{}] has been murdered.", GetCharacterName(character));
                        const auto assassinPlayer = playerByCharacter_.find(Character::ASSASSIN);
                        const auto victimPlayer = playerByCharacter_.find(character);
                        assert(assassinPlayer != playerByCharacter_.end());
                        assert(victimPlayer != playerByCharacter_.end());
                        if (assassinPlayer != playerByCharacter_.end() && victimPlayer != playerByCharacter_.end())
                        {
                            spdlog::debug("[{}] has been murdered by [{}] !", victimPlayer->second->GetName(), assassinPlayer->second->GetName());
                        }
                    }
                    // Current player skip it's turn
                    continue;
                }
                else if (offenser == Character::THIEF)
                {
                    auto thiefPlayer = playerByCharacter_.find(Character::THIEF);
                    auto victimPlayer = playerByCharacter_.find(character);

                    {
                        // Debug block
                        spdlog::debug("[{}] has been stolen !", GetCharacterName(character));
                        assert(thiefPlayer != playerByCharacter_.end());
                        assert(victimPlayer != playerByCharacter_.end());
                        if (thiefPlayer != playerByCharacter_.end() && victimPlayer != playerByCharacter_.end())
                        {
                            spdlog::debug("[{}] has been stolen by [{}] !", victimPlayer->second->GetName(), thiefPlayer->second->GetName());
                        }
                    }

                    const auto stolenGold = victimPlayer->second->GetGoldCoins();
                    thiefPlayer->second->ModifyGoldCoins(stolenGold);
                    victimPlayer->second->ModifyGoldCoins(-stolenGold);
                }
            }

            // Note Player uses his magic power whenever he wants but will be requested after building
            // 1) Take an action: player earns 2 gold coins OR watch 2 district cards, pick one, place the other to district cards heap bottom
            // 2) Build a district card from his hand
            // 3) Any moment during player turn, magic power can be used (if any)
            // 4) Any moment during player turn, income can be received (if any)
            PlayerTurn(player);
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
                spdlog::error("Error: CanUseMagicPower called with Character::UNINITIALIZED");
                assert(!"CanUseMagicPower called with Character::UNINITIALIZED");
                return false;
            }
            default:;
        }
        return true;
    }

    std::vector<const Player*> Boardgame::GetOpponentPlayers(const int playerID)
    {
        std::vector<const Player*> result;
        for (const auto& pair : playerById_)
        {
            if (pair.first != playerID)
            {
                result.push_back(pair.second.get());
            }
        }
        return result;
    }

    bool Boardgame::TakeGoldCoins(Player* player)
    {
        assert(player != nullptr);
        player->ModifyGoldCoins(2);
        return true;
    }

    bool Boardgame::WatchPickDistrict(Player* player)
    {
        assert(player != nullptr);

        const size_t numberOfDistricts = 2;
        const auto peekedDistricts = districtDeck_.GetDistricts(DistrictDeckAction::PEEK, numberOfDistricts);

        // Player can watch 2 cards but pick only one.
        const auto selectedDistrict = player->WatchAndChooseDistrictCard(peekedDistricts);

        // Check player isn't cheating.
        const auto peekedDistrictsIt = std::find(std::begin(peekedDistricts), std::end(peekedDistricts), selectedDistrict);
        if (peekedDistrictsIt == std::end(peekedDistricts))
        {
            spdlog::error("Player must pick a district among proposed ones");
            return false;
        }

        // Transfer the card to player
        player->GetAvailableDistricts().push_back(selectedDistrict);

        auto drawnDistricts = districtDeck_.GetDistricts(DistrictDeckAction::DRAW, numberOfDistricts);
        assert(peekedDistricts == drawnDistricts);

        const auto drawnDistrictsIt = std::find(std::begin(drawnDistricts), std::end(drawnDistricts), selectedDistrict);
        if (drawnDistrictsIt != std::end(drawnDistricts))
        {
            drawnDistricts.erase(drawnDistrictsIt); // Remove chosen card
            districtDeck_.Discard(drawnDistricts);  // Add the rest
        }
        else
        {
            spdlog::error("Could not find district [{}] in drawn districts.", GetDistrictName(selectedDistrict));
        }

        return true;
    }

    bool Boardgame::BuildDistrict(Player* player)
    {
        assert(player != nullptr);

        // Ask player for building
        const size_t authorizedBuilds = GetNumberOfAuthorizedBuilds(player->GetCharacter());
        auto districtCards = player->ChooseDistrictCardsToBuild(authorizedBuilds);

        if (districtCards.empty())
        {
            spdlog::debug("Player [{}] do not build during this round.", player->GetName());
            return true;
        }

        if (districtCards.size() > authorizedBuilds)
        {
            spdlog::error("Player [{}] is not able to build [{}] districts.", player->GetName(), districtCards.size());
            return false;
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
            spdlog::error("Player [{}] tried to build but doesn't have enough gold coins.", player->GetName());
            return false;
        }

        // Finally build the district
        player->BuildDistrict(districtCards);

        // Record first player ending game to grant bonus points
        if (player->GetBuiltCitySize() >= numberOfDistrictsToWin_ && firstPlayerEndingGame_ == -1)
        {
            spdlog::debug("[{}] is first player to end it's city ({}) districts built.", player->GetName(), numberOfDistrictsToWin_);
            firstPlayerEndingGame_ = player->GetID();
        }

        return true;
    }

    bool Boardgame::UseMagicPower(Player* player)
    {
        const auto character = player->GetCharacter();
        switch (character)
        {
            case Character::ASSASSIN:
            case Character::THIEF:
            {
                return AskCharacterTarget(player);
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
                spdlog::error("No magic power for [{}]", GetCharacterName(player->GetCharacter()));
            }
        }
        return true;
    }

    bool Boardgame::EarnDistrictIncome(Player* player)
    {
        const Color playerColor = GetCharacterColor(player->GetCharacter());

        if (playerColor == Color::UNINITIALIZED)
        {
            return true;
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

        return true;
    }

    void Boardgame::ActionStepTransition(Player* player)
    {
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
    }

    void Boardgame::BuildStepTransition(Player* player)
    {
    }

    void Boardgame::MagicPowerStepTransition(Player* player)
    {
    }

    void Boardgame::DistrictIncomeStepTransition(Player* player)
    {
    }

    void Boardgame::PlayerTurn(Player* player)
    {
        // TODO: Move these data to relevant place
        static const std::map<PlayerTurnStep, std::vector<PlayerAction>> actionsMapping
        {
            { PlayerTurnStep::ACTION_STEP, { PlayerAction::TAKE_GOLD_COINS, PlayerAction::WATCH_DISTRICT_CARDS } },
            { PlayerTurnStep::BUILD_STEP, { PlayerAction::BUILD_DISTRICT_CARDS } },
            { PlayerTurnStep::MAGIC_POWER_STEP, { PlayerAction::USE_MAGIC_POWER } },
            { PlayerTurnStep::DISTRICT_INCOME_STEP, { PlayerAction::EARN_DISTRICT_INCOME } },
        };

        static const std::map<PlayerAction, PlayerTurnStep> stepByAction
        {
            { PlayerAction::TAKE_GOLD_COINS, PlayerTurnStep::ACTION_STEP },
            { PlayerAction::WATCH_DISTRICT_CARDS, PlayerTurnStep::ACTION_STEP },
            { PlayerAction::BUILD_DISTRICT_CARDS, PlayerTurnStep::BUILD_STEP },
            { PlayerAction::USE_MAGIC_POWER, PlayerTurnStep::MAGIC_POWER_STEP },
            { PlayerAction::EARN_DISTRICT_INCOME, PlayerTurnStep::DISTRICT_INCOME_STEP },
        };

        using PairOfFunctions = std::pair<std::function<bool(Boardgame&, Player*)>, std::function<void (Boardgame&, Player*)>>;
        static const std::map<PlayerAction, PairOfFunctions> functionByAction
        {
            { PlayerAction::TAKE_GOLD_COINS, { std::mem_fn(&Boardgame::TakeGoldCoins), std::mem_fn(&Boardgame::ActionStepTransition) } },
            { PlayerAction::WATCH_DISTRICT_CARDS, { std::mem_fn(&Boardgame::WatchPickDistrict), std::mem_fn(&Boardgame::ActionStepTransition) } },
            { PlayerAction::BUILD_DISTRICT_CARDS, { std::mem_fn(&Boardgame::BuildDistrict), std::mem_fn(&Boardgame::BuildStepTransition) } },
            { PlayerAction::USE_MAGIC_POWER, { std::mem_fn(&Boardgame::UseMagicPower), std::mem_fn(&Boardgame::MagicPowerStepTransition) } },
            { PlayerAction::EARN_DISTRICT_INCOME, { std::mem_fn(&Boardgame::EarnDistrictIncome), std::mem_fn(&Boardgame::DistrictIncomeStepTransition) } },
        };

        std::map<PlayerTurnStep, bool> stepState
        {
            { PlayerTurnStep::ACTION_STEP, true },
            { PlayerTurnStep::BUILD_STEP, true },
            { PlayerTurnStep::MAGIC_POWER_STEP, CanUseMagicPower(player->GetCharacter()) },
            { PlayerTurnStep::DISTRICT_INCOME_STEP, true },
        };

        auto allStepsAreDone = [&stepState]()
        {
            for (const auto& pair : stepState)
            {
                if (pair.second == true)
                {
                    return false;
                }
            }
            return true;
        };

        do
        {
            std::vector<PlayerAction> availableActions;
            for (const auto& step : stepState)
            {
                if (step.second)
                {
                    const auto it = actionsMapping.find(step.first);
                    if (it != std::end(actionsMapping))
                    {
                        availableActions.insert(std::begin(availableActions), std::begin(it->second), std::end(it->second));
                    }
                }
            }
            assert(availableActions.empty() == false);
            assert(std::find(std::begin(availableActions), std::end(availableActions), PlayerAction::UNINITIALIZED) == std::end(availableActions));

            PlayerAction action = PlayerAction::UNINITIALIZED;
            bool validAction = false;
            size_t attempts = 0;
            static const size_t maximumAttempts = 3;
            do
            {
                // Ask player to choose an action
                action = player->ChooseAction(availableActions);
                // Ensure action is available
                validAction = std::find(std::begin(availableActions), std::end(availableActions), action) != std::end(availableActions);
            }
            while (validAction == false && attempts++ < maximumAttempts);

            if (validAction == false)
            {
                spdlog::error("Maximum attempts [{}] reached to choose action", attempts);
                return;
            }

            // Which PlayerTurnStep will be consumed by PlayerAction ?
            const auto stepActionIt = stepByAction.find(action);
            if (stepActionIt == std::end(stepByAction))
            {
                spdlog::critical("Cannot find value from PlayerAction key [{}]", GetPlayerActionName(action));
                return;
            }

            auto stepStateIt = stepState.find(stepActionIt->second);
            if (stepStateIt == std::end(stepState))
            {
                spdlog::critical("Cannot find value from PlayerTurnStep key [{}]", GetPlayerTurnStepName(stepActionIt->second));
                return;
            }

            auto functionActionIt = functionByAction.find(action);
            if (functionActionIt != std::end(functionByAction))
            {
                PairOfFunctions pair = functionActionIt->second;

                // Call state processing function
                const bool result = pair.first(*this, player);
                if (result)
                {
                    // This step is now consumed
                    spdlog::debug("Step [{}] is now consumed for [{}]", GetPlayerTurnStepName(stepActionIt->second), player->GetName());
                    stepStateIt->second = false;

                    // Call state transition function
                    pair.second(*this, player);
                }
            }
        }
        while (allStepsAreDone() == false);
    }

    bool Boardgame::AskCharacterTarget(Player* player)
    {
        const auto possibleVictims = characterDeck_.GetOpponentCharacters(player->GetCharacter());
        const auto victim = player->ChooseCharacterTarget(possibleVictims);
        if (possibleVictims.find(victim) == std::end(possibleVictims))
        {
            spdlog::warn("Player [{}] choosed [{}] but it's impossible.", player->GetName(), GetCharacterName(victim));
        }
        else if (victims.find(victim) != std::end(victims))
        {
            spdlog::warn("Character [{}] is already a victim. Cannot target a victim.", GetCharacterName(victim));
        }
        else
        {
            assert(player->GetCharacter() != Character::UNINITIALIZED);
            victims.insert({ victim, player->GetCharacter() });
        }

        return true;
    }

    bool Boardgame::MagicianMagicPower(Player* player)
    {
        // Three possibilities:
        // 1) Exchange your entire hand of cards(not the cards in your city) with the hand of another player
        // 2)  Place any number of cards from your hand facedown at the bottom of the District Deck, and
        //     then draw an equal number of cards from the top of the District Deck
        // 3) Do nothing
        const auto opponents = GetOpponentPlayers(player->GetID());
        const MagicianChoice magicianChoice = player->MagicianDecision(opponents);
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
                spdlog::error("Magician [{}] choice is not correct: [{}]", player->GetName(), static_cast<int>(magicianChoice));
                return false;
            }
        }

        return true;
    }

    bool Boardgame::MagicianExchangeFromPlayer(Player* player)
    {
        const auto opponents = GetOpponentPlayers(player->GetID());
        const int victimID = player->ChoosePlayerTarget(opponents);

        if (victimID == player->GetID())
        {
            spdlog::error("Player [{}] cannot self swap available districts", player->GetName());
            return false;
        }

        auto playerIdPairIt = playerById_.find(victimID);
        if (playerIdPairIt == playerById_.end())
        {
            spdlog::error("Unable to find [{}] player ID. Retry.", victimID);
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
            spdlog::error("Player [{}] cannot swap with districts deck, no district to swap !", player->GetName());
            return false;
        }

        if (districtDeck_.GetPileOfCardSize() < cardsInHand.size())
        {
            spdlog::error("Player [{}] cannot swap with districts deck, there is not enough districts in deck to swap", player->GetName());
            return false;
        }

        auto districtsToDiscard = player->ChooseDistrictsCardsToSwap();
        if (districtsToDiscard.empty())
        {
            spdlog::error("Player [{}] returned no districts to exchange with District deck", player->GetName());
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
                spdlog::error("Player [{}] chose district [{}] he does not own", player->GetName(), GetDistrictName(*it));
                // Insert removed cards to rollback
                cardsInHand.insert(cardsInHand.end(), districtsToDiscard.begin(), it);
                return false;
            }
        }

        districtDeck_.Discard(districtsToDiscard);
        auto drawnCards = districtDeck_.GetDistricts(DistrictDeckAction::DRAW, districtsToDiscard.size());
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
        if (pair.first == -1)
        {
            spdlog::debug("Player [{}] finally doesn't destroy a district", player->GetName());
            return true;
        }

        // Find player
        auto victimIt = playerById_.find(pair.first);
        if (victimIt == playerById_.end())
        {
            spdlog::error("Unable to find player id [{}]", pair.first);
            return true;
        }

        // Warlord cannot destroy Bishop districts
        const auto bishopIt = playerByCharacter_.find(Character::BISHOP);
        if (bishopIt != std::end(playerByCharacter_))
        {
            if (bishopIt->second->GetID() == victimIt->second->GetID())
            {
                spdlog::warn("Player [{}] cannot destroy Bishop district", player->GetName());
                return true;
            }
        }

        // Once a city is completed, this city becomes immune to Warlord
        if (victimIt->second->GetBuiltCitySize() >= numberOfDistrictsToWin_)
        {
            spdlog::warn("Player [{}] cannot destroy a district in a finished Citadel", player->GetName());
            return true;
        }

        if (pair.second == District::UNINITIALIZED)
        {
            spdlog::error("Player [{}] cannot destroy UNINITIALIZED district", player->GetName());
            return true;
        }

        if (pair.second == District::KEEP)
        {
            spdlog::warn("Player [{}] cannot destroy KEEP district", player->GetName());
            return true;
        }

        if (GetDistrictCost(pair.second) >= 1)
        {
            // Destroy cost is district cost minus one.
            if (GetDistrictCost(pair.second) - 1 <= player->GetGoldCoins())
            {
                if (victimIt->second->DestroyDistrict(pair.second) == false)
                {
                    spdlog::warn("Player [{}] does not have [{}]", pair.first, GetDistrictName(pair.second));
                    return true;
                }
                spdlog::info("Player [{}] destroyed [{}] owned by [{}]", player->GetName(), GetDistrictName(pair.second), victimIt->second->GetName());
            }
            else
            {
                spdlog::warn("Player [{}] has not enough gold to destroy this district", pair.first);
                return true;
            }
        }
        else
        {
            spdlog::critical("District [{}] has a cost of [{}] gold coins.", GetDistrictName(pair.second), GetDistrictCost(pair.second));
            return false;
        }

        return true;
    }

    bool Boardgame::IsGameEnded() const
    {
        if (districtDeck_.IsPileEmpty())
        {
            spdlog::debug("Game ended, pile of cards is empty.");
            return true;
        }

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
            if (player->GetID() == firstPlayerEndingGame_)
            {
                score += 4;
            }
            // Rule 4
            else if (city.size() >= numberOfDistrictsToWin_)
            {
                score += 2;
            }

            scores[player->GetID()] = score;

            spdlog::info("Player [{}] has [{}] points.", player->GetName(), score);
        }
        // TODO: display player scores
    }
}
