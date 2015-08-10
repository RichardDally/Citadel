#pragma once

#include <map>

namespace Citadel
{
    namespace CardFactory
    {
        template <typename Key, typename Value>
        const Value& GetCard(const Key key)
        {
            static std::map<Key, Value> cache;

            auto it = cache.find(key);
            if (it != cache.end())
            {
                return it->second;
            }
            else
            {
                cache.insert(std::make_pair(key, Value(key)));
                return cache.at(key);
            }
        }
    }
}