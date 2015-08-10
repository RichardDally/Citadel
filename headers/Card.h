#pragma once

class Card
{
public:
    Card(const char* const name, const char* const description)
        : name_(name), description_(description)
    {
    }

    const char* const GetDescription() const
    {
        return description_;
    }

private:
    const char* const name_ = nullptr;
    const char* const description_ = nullptr;
};
