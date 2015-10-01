#include <algorithm>
#include "Logger.h"
#include "Randomness.h"
#include "RobotPlayer.h"

namespace Citadel
{
    namespace
    {
        const char* const names[] =
        {
            "Hal",
            "Synapse",
            "DevNull",
            "Alpha",
            "TheOracle",
            "Alfred",
            "Corona",
        };
        const size_t namesSize = sizeof(names) / sizeof(names[0]);
    }

    RobotPlayer::RobotPlayer(const std::string& name)
        : Player(name)
    {
        assert(name.empty() == false);
    }

#pragma region PURE VIRTUAL METHODS
    // Returns character picked to play
    Character RobotPlayer::PickCharacter(const std::set<Character>& remainingCharacters, const std::set<Character>& faceupCharacters)
    {
        assert(remainingCharacters.size() > 0);
        if (remainingCharacters.empty())
        {
            Logger::GetInstance() << Verbosity::ERROR << "There is no remaining characters" << std::endl;
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
            Logger::GetInstance() << Verbosity::DEBUG << "[" << GetCharacterName(result) << "] is the most valuable for [" << GetName() << "]" << std::endl;
        }
        else
        {
            auto random_it = std::next(std::begin(remainingCharacters), Dice::GetRandomNumber(0, remainingCharacters.size() - 1));
            result = *random_it;
            Logger::GetInstance() << Verbosity::DEBUG << "[" << GetName() << "] picked randomly [" << GetCharacterName(result) << "]" << std::endl;
        }

        return result;
    }

    PlayerAction RobotPlayer::ChooseAction(const std::vector<PlayerAction>& availableActions)
    {
        assert(availableActions.empty() == false);
        if (availableActions.empty())
        {
            Logger::GetInstance() << Verbosity::FATAL << "There is no available action." << std::endl;
            return PlayerAction::UNINITIALIZED;
        }

        // TODO: optimize
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
            Logger::GetInstance() << Verbosity::ERROR << "There is no district to watch and choose." << std::endl;
            return District::UNINITIALIZED;
        }

        for (const auto district : districts)
        {
            if (std::find(std::begin(GetBuiltCity()), std::end(GetBuiltCity()), district) == std::end(GetBuiltCity()))
            {
                Logger::GetInstance() << Verbosity::DEBUG << "[" << GetName() << "] hasn't this card [" << GetDistrictName(district) << "]" << std::endl;
                return district;
            }
        }
        const auto result = districts[Dice::GetRandomNumber(0, districts.size() - 1)];
        Logger::GetInstance() << Verbosity::DEBUG << "[" << GetName() << "] picked randomly [" << GetDistrictName(result) << "]" << std::endl;
        return result;
    }

    // Returns districts player wants to build
    std::vector<District> RobotPlayer::ChooseDistrictCardsToBuild(const size_t authorizedBuilds)
    {
        assert(!"Not implemented yet");
        return std::vector<District>();
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
                Logger::GetInstance() << Verbosity::DEBUG << "Robot [" << GetName() << "] targets [" << GetCharacterName(character) << "]" << std::endl;
                return character;
            }
        }

        Logger::GetInstance() << Verbosity::FATAL << "Robot [" << GetName() << "] didn't find a target." << std::endl;
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

        Logger::GetInstance() << Verbosity::DEBUG << "Player [" << GetName() << "] have not found any 1 gold district to destroy..." << std::endl;
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

    // Returns discarded districts to be replaced by equivalent number of cards from district deck
    std::vector<District> RobotPlayer::ChooseDistrictsCardsToSwap()
    {
        assert(!"Not implemented yet");
        return std::vector<District>();
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