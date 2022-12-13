#pragma once
#include <unordered_map>

template <typename KeyT, typename ValueT>
struct DictWithDefault
{
    std::unordered_map<KeyT, ValueT> dict;
    ValueT defaultValue;

    explicit DictWithDefault(ValueT defaultValue_)
        : defaultValue(std::move(defaultValue_))
    {
    }

    void Set(KeyT const &key, ValueT value)
    {
        dict[key] = std::move(value);
    }

    ValueT Get(KeyT const &key) const
    {
        if (auto iter = dict.find(key); iter != end(dict))
            return iter->second;

        return defaultValue;
    }
};
