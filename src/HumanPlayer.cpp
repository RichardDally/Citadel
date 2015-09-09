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
    PlayerAction HumanPlayer::ChooseAction(const std::vector<PlayerAction>& availableActions)
    {
        std::cout << "@" << GetName() << ", You currently have " << GetGoldCoins() << " gold coins." << std::endl;
        std::cout << "@" << GetName() << ", Select player action among: " << std::endl;

        const size_t size = availableActions.size();
        for (size_t i = 0; i < size; ++i)
        {
            std::cout << "- " << GetPlayerActionName(availableActions[i]) << " (" << i << ")" << std::endl;
        }

        size_t playerActionChoice = 0;
        std::cin >> playerActionChoice;

        if (playerActionChoice < availableActions.size())
        {
            return availableActions[playerActionChoice];
        }
        return PlayerAction::UNINITIALIZED;
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

        const size_t size = availableDistricts_.size();
        for (size_t i = 0; i < size; ++i)
        {
            DisplayDistrictDetails(availableDistricts_[i], i);
        }
        std::cout << "- Stop (" << availableDistricts_.size() << ")" << std::endl;

        return SelectMultipleDistricts(availableDistricts_);
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
            std::cout << "- " << player->GetName() << ", " << player->GetNumberOfAvailableDistricts() << " cards in hand (" << indexedPlayers.size() << ")" << std::endl;
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
    std::pair<int, District> HumanPlayer::ChoosePlayerDistrictTarget(std::vector<const Player*> players)
    {
        std::cout << "@" << GetName() << ", Choose a player id followed by a district id, among:" << std::endl;

        static_assert(std::is_same<decltype(Player::GetID()), int>::value, "GetID must return an integer");
        bool somethingToDestroy = false;
        const size_t playersSize = players.size();
        for (size_t playerIndex = 0; playerIndex < playersSize; ++playerIndex)
        {
            std::cout << "Player " << players[playerIndex]->GetName() << " (" << playerIndex << ")" << std::endl;
            const auto& city = players[playerIndex]->GetBuiltCity();
            if (city.empty() == false)
            {
                somethingToDestroy = true;
            }
            for (const auto district : city)
            {
                DisplayDistrictDetails(district, static_cast<size_t>(district));
            }
        }

        if (somethingToDestroy)
        {
            size_t playerIndex = 0;
            size_t districtID = 0;
            std::cin >> playerIndex >> districtID;

            if (playerIndex < players.size())
            {
                const Player* targetedPlayer = players[playerIndex];
                if (districtID < static_cast<size_t>(District::MAX))
                {
                    return{ targetedPlayer->GetID(), static_cast<District>(districtID) };
                }
                else
                {
                    std::cerr << "District ID [" << districtID << "] does not exist." << std::endl;
                }
            }
            else
            {
                std::cerr << "Player index [" << playerIndex << "] is not valid, there is only [" << players.size() << "] players available to choose." << std::endl;
            }
        }
        else
        {
            std::cout << "All cities are empty, there is nothing to destroy." << std::endl;
        }

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

        const size_t size = availableDistricts_.size();
        for (size_t i = 0; i < size; ++i)
        {
            DisplayDistrictDetails(availableDistricts_[i], i);
        }
        std::cout << "- Stop (" << availableDistricts_.size() << ")" << std::endl;

        return SelectMultipleDistricts(availableDistricts_);
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