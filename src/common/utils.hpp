#pragma once
#include <algorithm>

template <typename T>
inline bool insert_sorted(std::vector<T> &container, T const &item)
{
    auto hint = std::lower_bound(begin(container), end(container), item);

    if (hint == end(container) || item < *hint)
    {
        container.insert(hint, item);
        return true;
    }

    return false;
}

template <typename T>
inline bool insert_sorted(std::vector<T> &container, T &&item)
{
    auto hint = std::lower_bound(begin(container), end(container), item);

    if (hint == end(container) || item < *hint)
    {
        container.insert(hint, std::move(item));
        return true;
    }

    return false;
}

template <typename T, typename FuncT>
inline void CombinationsWithReplacement(std::vector<T> const &list, std::size_t length, FuncT &&func)
{
    std::size_t const nb = list.size();
    std::vector<T> values(length);
    std::vector<std::size_t> indices(length);

    for (std::size_t i = 0; i < length; ++i)
    {
        values[i] = list[indices[i]];
    }

    func(values);

    while (true)
    {
        std::size_t i = length - 1;
        for (; i < length; --i)
        {
            if (indices[i] != nb - 1)
                break;
        }

        if (i > length)
            return;

        std::size_t const nextIdx = indices[i] + 1;
        for (std::size_t j = i; j < length; ++j)
        {
            indices[j] = nextIdx;
            values[j] = list[nextIdx];
        }

        func(values);
    }
}
