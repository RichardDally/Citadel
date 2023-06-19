#include <algorithm>
#include <spdlog/spdlog.h>
#include "Randomness.h"
#include "RobotPlayer.h"


namespace Citadel
{
    RobotPlayer::RobotPlayer(const std::string& name)
        : Player(name)
    {
        if (name.empty())
        {
            throw std::runtime_error("Name is empty");
        }
    }

#pragma region PURE VIRTUAL METHODS
    // Returns character picked to play
    Character RobotPlayer::PickCharacter(const std::set<Character>& remainingCharacters, const std::set<Character>& faceupCharacters)
    {
        assert(remainingCharacters.size() > 0);
        if (remainingCharacters.empty())
        {
            spdlog::error("There is no remaining characters");
            return Character::UNINITIALIZED;
        }

        Character result = Character::UNINITIALIZED;
        std::map<Character, size_t> incomeByCharacter;
        std::pair<Character, size_t> mostValuableCharacter = std::make_pair(Character::UNINITIALIZED, 0);
        for (const auto character : remainingCharacters)
        {
            const auto goldIncome = SimulateDistrictIncome(character);
            if (goldIncome > mostValuableCharacter.second)
            {
                mostValuableCharacter = std::make_pair(character, goldIncome);
            }
            incomeByCharacter.insert(std::make_pair(character, goldIncome));
        }

        if (mostValuableCharacter.first != Character::UNINITIALIZED)
        {
            result = mostValuableCharacter.first;
            spdlog::debug("[{}] is the most valuable for [{}]", GetCharacterName(result), GetName());
        }
        else
        {
            auto random_it = std::next(std::begin(remainingCharacters), Dice::GetRandomNumber(0, remainingCharacters.size() - 1));
            result = *random_it;
            spdlog::debug("[{}] picked randomly [{}]", GetName(), GetCharacterName(result));
        }

        return result;
    }

    PlayerAction RobotPlayer::ChooseAction(const std::vector<PlayerAction>& availableActions)
    {
        assert(availableActions.empty() == false);
        if (availableActions.empty())
        {
            spdlog::critical("There is no available action.");
            return PlayerAction::UNINITIALIZED;
        }

        if (availableActions.size() == 1)
        {
            return availableActions.front();
        }

        if (std::find(std::begin(availableActions), std::end(availableActions), PlayerAction::BUILD_DISTRICT_CARDS) != std::end(availableActions) && CanBuild())
        {
            return PlayerAction::BUILD_DISTRICT_CARDS;
        }
        else if (std::find(std::begin(availableActions), std::end(availableActions), PlayerAction::EARN_DISTRICT_INCOME) != std::end(availableActions))
        {
            return PlayerAction::EARN_DISTRICT_INCOME;
        }
        else if (std::find(std::begin(availableActions), std::end(availableActions), PlayerAction::WATCH_DISTRICT_CARDS) != std::end(availableActions) && GetAvailableDistricts().empty())
        {
            return PlayerAction::WATCH_DISTRICT_CARDS;
        }
        else if (std::find(std::begin(availableActions), std::end(availableActions), PlayerAction::TAKE_GOLD_COINS) != std::end(availableActions))
        {
            return PlayerAction::TAKE_GOLD_COINS;
        }
        else if (std::find(std::begin(availableActions), std::end(availableActions), PlayerAction::USE_MAGIC_POWER) != std::end(availableActions))
        {
            return PlayerAction::USE_MAGIC_POWER;
        }

        assert(!"Must return valid PlayerAction");
        return PlayerAction::UNINITIALIZED;
    }

    District RobotPlayer::WatchAndChooseDistrictCard(const std::vector<District>& districts)
    {
        assert(districts.empty() == false);
        if (districts.empty())
        {
            spdlog::error("There is no district to watch and choose.");
            return District::UNINITIALIZED;
        }

        for (const auto district : districts)
        {
            if (std::find(std::begin(GetBuiltCity()), std::end(GetBuiltCity()), district) == std::end(GetBuiltCity()))
            {
                spdlog::debug("[{}] hasn't this card [{}]", GetName(), GetDistrictName(district));
                return district;
            }
        }
        const auto result = districts[Dice::GetRandomNumber(0, districts.size() - 1)];
        spdlog::debug("[{}] picked randomly [{}]", GetName(), GetDistrictName(result));
        return result;
    }

    std::vector<District> RobotPlayer::ChooseDistrictCardsToBuild(const size_t authorizedBuilds)
    {
        std::vector<District> result;

        // TODO: improve AI to build multiple districts
        auto districtToBuild = District::UNINITIALIZED;
        for (const auto district : GetAvailableDistricts())
        {
            const auto cost = GetDistrictCost(district);
            if (GetGoldCoins() >= cost)
            {
                if (cost > GetDistrictCost(districtToBuild))
                {
                    if (std::find(std::begin(GetBuiltCity()), std::end(GetBuiltCity()), district) == std::end(GetBuiltCity()))
                    {
                        districtToBuild = district;
                    }
                }
            }
        }

        if (districtToBuild != District::UNINITIALIZED)
        {
            result.push_back(districtToBuild);
        }
        return result;
    }

    // Returns character targeted by assassination or theft
    Character RobotPlayer::ChooseCharacterTarget(const std::set<Character>& opponents)
    {
        // TODO: improve and make dynamic weight for each targets depending on game state
        static const std::vector<Character> topTargets
        {
            Character::MERCHANT, // Greedy basterd
            Character::ARCHITECT, // Crazy builder
            Character::MAGICIAN, // Card stealer
            Character::WARLORD,  // District demolisher
            Character::KING, // Crown stealer
            Character::BISHOP,
            Character::THIEF,
        };

        // Target most interesting character
        for (const auto character : topTargets)
        {
            if (std::find(std::begin(opponents), std::end(opponents), character) != std::end(opponents))
            {
                spdlog::info("Robot [{}] targets [{}]", GetName(), GetCharacterName(character));
                return character;
            }
        }

        spdlog::critical("Robot [{}] didn't find a target.", GetName());
        return Character::UNINITIALIZED;
    }

    // Returns opponent player id, current player wants to target
    int RobotPlayer::ChoosePlayerTarget(const std::vector<const Player*>& opponents)
    {
        int result = -1;
        size_t maxAvailable = 0;

        // Target player with a lot of available districts
        assert(opponents.empty() == false);
        for (const auto player : opponents)
        {
            if (player->GetNumberOfAvailableDistricts() >= maxAvailable)
            {
                result = player->GetID();
                maxAvailable = player->GetNumberOfAvailableDistricts();
            }
        }
        assert(result != -1);
        return result;
    }

    std::pair<int, District> RobotPlayer::ChoosePlayerDistrictTarget(const std::vector<const Player*>& players)
    {
        for (const auto player : players)
        {
            if (player->GetID() != GetID())
            {
                for (const auto district : player->GetBuiltCity())
                {
                    if (GetDistrictCost(district) == 1)
                    {
                        return{ player->GetID(), district };
                    }
                }
            }
        }

        spdlog::debug("Player [{}] have not found any 1 gold district to destroy...", GetName());
        return std::pair<int, District>(-1, District::UNINITIALIZED);
    }

    MagicianChoice RobotPlayer::MagicianDecision(const std::vector<const Player*>& opponents)
    {
        if (GetNumberOfAvailableDistricts() == 0)
        {
            return MagicianChoice::EXCHANGE_FROM_PLAYER;
        }
        else
        {
            // Exchange from district deck choice may be not interesting (TODO: use pileSize argument)
            for (const auto district : GetAvailableDistricts())
            {
                if (std::find(std::begin(GetBuiltCity()), std::end(GetBuiltCity()), district) != std::end(GetBuiltCity()))
                {
                    return MagicianChoice::EXCHANGE_FROM_DISTRICT_DECK;
                }
            }
        }
        return MagicianChoice::DO_NOTHING;
    }

    std::vector<District> RobotPlayer::ChooseDistrictsCardsToSwap()
    {
        std::vector<District> result;

        for (const auto district : GetAvailableDistricts())
        {
            if (std::find(std::begin(GetBuiltCity()), std::end(GetBuiltCity()), district) != std::end(GetBuiltCity()))
            {
                result.push_back(district);
            }
        }

        assert(result.empty() == false);
        return result;
    }
#pragma endregion

    const size_t RobotPlayer::SimulateDistrictIncome(const Character character) const
    {
        const auto characterColor = GetCharacterColor(character);
        if (characterColor == Color::UNINITIALIZED)
        {
            return 0;
        }

        size_t goldIncome = 0;
        for (const auto district : builtCity_)
        {
            if (GetDistrictColor(district) == characterColor)
            {
                ++goldIncome;
            }
        }
        return goldIncome;
    }

    bool RobotPlayer::CanBuild() const
    {
        for (const auto district : builtCity_)
        {
            if (GetDistrictCost(district) <= GetGoldCoins())
            {
                return true;
            }
        }
        return false;
    }
}
