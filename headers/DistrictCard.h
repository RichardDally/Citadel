#pragma once

#include "Card.h"
#include "GameData.h"

class DistrictCard : public Card
{
public:
    DistrictCard(const char* const name, const char* const description, const int cost, const Color color)
        : Card(name, description), cost_(cost), color_(color)
    {
    }

    const int GetCost() const
    {
        return cost_;
    }

    const Color GetColor() const
    {
        return color_;
    }

private:
    const int cost_;
    const Color color_;
};