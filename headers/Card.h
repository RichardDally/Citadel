#pragma once

class Card
{
public:
    Card(const char* const name, const char* const description)
        : id_(cardIdGenerator_++), name_(name), description_(description)
    {
    }

    const int GetID() const
    {
        return id_;
    }

    const char* const GetDescription() const
    {
        return description_;
    }

private:
    static int cardIdGenerator_;
    const int id_ = -1;
    const char* const name_ = nullptr;
    const char* const description_ = nullptr;
};
