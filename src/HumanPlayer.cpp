#include <set>
#include <array>
#include <iostream>
#include <unordered_set>
#include "HumanPlayer.h"

namespace Citadel
{
    HumanPlayer::HumanPlayer()
    {
        name_ = GetID();

        //std::cout << "Enter player " << GetID() << " name: ";
        //std::cin >> name_;
    }

    HumanPlayer::HumanPlayer(const std::string& name)
        : Player(name)
    {
    }

    // Returns character picked to play
    Character HumanPlayer::PickCharacter(const std::set<Character>& remainingCards)
    {
        assert(remainingCards.size() > 0);
        std::vector<Character> indexedCharacters;
        
        std::cout << "@" << GetName() << ", Select character among: " << std::endl;
        for (const auto character : remainingCards)
        {
            std::cout << "- " << GetCharacterName(character) << " (" << indexedCharacters.size() << ")" << std::endl;
            indexedCharacters.push_back(character);
        }
        size_t characterChoice = 0;
        std::cin >> characterChoice;

        if (characterChoice < indexedCharacters.size())
        {
            return indexedCharacters[characterChoice];
        }
        return Character::UNINITIALIZED;
    }

    // Returns action to be taken
    PlayerAction HumanPlayer::ChooseAction(const PlayerTurnStep step, const bool canUseMagicPower)
    {
        std::cout << "@" << GetName() << ", You currently have " << GetGoldCoins() << " gold coins." << std::endl;
        std::cout << "Current turn step is " << GetPlayerTurnStepName(step) << std::endl;

        std::vector<PlayerAction> indexedAction;

        std::cout << "@" << GetName() << ", Select player action among: " << std::endl;
        switch (step)
        {
            case PlayerTurnStep::ACTION_STEP:
            {
                indexedAction.push_back(PlayerAction::TAKE_GOLD_COINS);
                indexedAction.push_back(PlayerAction::WATCH_DISTRICT_CARDS);
                break;
            }
            case PlayerTurnStep::BUILD_STEP:
            {
                indexedAction.push_back(PlayerAction::BUILD_DISTRICT_CARDS);
                break;
            }
            default:;
        }

        if (canUseMagicPower)
        {
            indexedAction.push_back(PlayerAction::USE_MAGIC_POWER);
        }

        const size_t size = indexedAction.size();
        for (size_t i = 0; i < size; ++i)
        {
            std::cout << "- " << GetPlayerActionName(indexedAction[i]) << " (" << i << ")" << std::endl;
        }

        size_t playerActionChoice = 0;
        std::cin >> playerActionChoice;

        if (playerActionChoice < indexedAction.size())
        {
            return indexedAction[playerActionChoice];
        }
        return PlayerAction::UNITIALIZED;
    }

    District HumanPlayer::WatchAndChooseDistrictCard(const std::vector<District>& districts)
    {
        std::cout << "@" << GetName() << ", You currently have " << GetGoldCoins() << " gold coins." << std::endl;
        std::cout << "@" << GetName() << ", Select one district among:" << std::endl;

        const size_t size = districts.size();
        for (size_t i = 0; i < size; ++i)
        {
            DisplayDistrictDetails(districts[i], i);
        }

        size_t districtChoice = 0;
        std::cin >> districtChoice;

        if (districtChoice < districts.size())
        {
            return districts[districtChoice];
        }

        return District::UNINITIALIZED;
    }

    std::vector<District> HumanPlayer::ChooseDistrictCardsToBuild(const size_t authorizedBuilds)
    {
        std::cout << "@" << GetName() << ", You currently have " << GetGoldCoins() << " gold coins." << std::endl;
        std::cout << "@" << GetName() << ", Select up to " << authorizedBuilds << " district among:" << std::endl;

        const size_t size = cardsInHand_.size();
        for (size_t i = 0; i < size; ++i)
        {
            DisplayDistrictDetails(cardsInHand_[i], i);
        }
        std::cout << "- Stop (" << cardsInHand_.size() << ")" << std::endl;

        return SelectMultipleDistricts(cardsInHand_);
    }

    // Returns character targeted by assassination or theft
    Character HumanPlayer::ChooseCharacterTarget(const std::set<Character>& opponents)
    {
        std::cout << "@" << GetName() << ", Choose a target among:" << std::endl;

        std::vector<Character> indexedCharacters;
        for (const auto character : opponents)
        {
            std::cout << "- " << GetCharacterName(character) << " (" << indexedCharacters.size() << ")" << std::endl;
            indexedCharacters.push_back(character);
        }

        size_t targetChoice = 0;
        std::cin >> targetChoice;

        if (targetChoice < indexedCharacters.size())
        {
            return indexedCharacters[targetChoice];
        }

        return Character::UNINITIALIZED;
    }

    // Returns opponent player id, current player wants to target
    int HumanPlayer::ChoosePlayerTarget(std::vector<const Player*> opponents)
    {
        std::cout << "@" << GetName() << ", Choose a target among:" << std::endl;

        std::vector<int> indexedPlayers;
        for (const auto player : opponents)
        {
            std::cout << "- " << player->GetName() << ", " << player->GetNumberOfCardsInHand() << " cards in hand (" << indexedPlayers.size() << ")" << std::endl;
            indexedPlayers.push_back(player->GetID());
        }

        size_t targetChoice = 0;
        std::cin >> targetChoice;

        if (targetChoice < indexedPlayers.size())
        {
            return indexedPlayers[targetChoice];
        }

        return -1;
    }

    // Returns a pair containing player id (self district destroy is tolerated) as key and destroyed district as value
    // TODO: test this method
    std::pair<int, District> HumanPlayer::ChoosePlayerDistrictTarget(std::vector<const Player*> players)
    {
        std::cout << "@" << GetName() << ", Choose a player id and a district, among:" << std::endl;

        static_assert(std::is_same<decltype(Player::GetID()), int>::value, "GetID must return an integer");
        for (const auto player : players)
        {
            std::cout << "Player " << player->GetName() << " (" << player->GetID() << ")" << std::endl;
            const auto& city = player->GetBuiltCity();
            for (const auto district : city)
            {
                DisplayDistrictDetails(district, static_cast<size_t>(district));
            }
        }

        std::string userInput;
        std::cin >> userInput;
        //std::istringstream stream(userInput);
        //std::istream_iterator<size_t> begin(stream), end;
        //std::vector<size_t> indexes(begin, end);

        /*
        // Player ID then district ID
        if (indexes.size() == 2)
        {
            if (indexes[0] < players.size())
            {
                const Player* targetedPlayer = players[indexes[0]];
                if (indexes[1] < static_cast<size_t>(District::MAX))
                {
                    return{ targetedPlayer->GetID(), static_cast<District>(indexes[1]) };
                }
            }
        }
        */

        return { -1, District::UNINITIALIZED };
    }

    // Returns a choice specific to Magician character
    MagicianChoice HumanPlayer::MagicianDecision()
    {
        std::cout << "@" << GetName() << ", As Magician make a choice among:" << std::endl;

        static const std::array<MagicianChoice, 3> indexedMagicianChoices { { MagicianChoice::DO_NOTHING, MagicianChoice::EXCHANGE_FROM_PLAYER, MagicianChoice::EXCHANGE_FROM_DISTRICT_DECK } };
        for (size_t i = 0; i < indexedMagicianChoices.size(); ++i)
        {
            std::cout << "- " << GetMagicianChoiceName(indexedMagicianChoices[i]) << " (" << i << ")" << std::endl;
        }
        
        size_t magicianChoice = 0;
        std::cin >> magicianChoice;

        if (magicianChoice < indexedMagicianChoices.size())
        {
            return indexedMagicianChoices[magicianChoice];
        }

        return MagicianChoice::DO_NOTHING;
    }

    // Returns discarded districts to be replaced by equivalent number of cards from district deck
    std::vector<District> HumanPlayer::ChooseDistrictsCardsToSwap()
    {
        std::cout << "@" << GetName() << ", Select district cards (one at a time, press enter between each) you want to swap among:" << std::endl;

        const size_t size = cardsInHand_.size();
        for (size_t i = 0; i < size; ++i)
        {
            DisplayDistrictDetails(cardsInHand_[i], i);
        }
        std::cout << "- Stop (" << cardsInHand_.size() << ")" << std::endl;

        return SelectMultipleDistricts(cardsInHand_);
    }

    std::vector<District> HumanPlayer::SelectMultipleDistricts(const std::vector<District>& container)
    {
        const size_t size = container.size();
        std::vector<District> result;
        std::unordered_set<size_t> alreadyChosen;
        size_t index = 0;
        while (std::cin >> index)
        {
            if (index < size)
            {
                if (alreadyChosen.find(index) == std::end(alreadyChosen))
                {
                    alreadyChosen.insert(index);
                    result.push_back(container[index]);
                }
            }
            else
            {
                break;
            }
        }

        return result;
    }

    void HumanPlayer::DisplayDistrictDetails(const District district, const size_t index) const
    {
        const char* const name = GetDistrictName(district);
        const char* const color = GetColorName(GetDistrictColor(district));
        const int cost = GetDistrictCost(district);
        std::cout << "- " << name << ", " << color << " district, costs " << cost << " gold coins. (" << index << ")" << std::endl;
    }
}