#pragma once
#include <algorithm>

template <typename T>
bool insert_sorted(std::vector<T> &container, T const &item)
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
bool insert_sorted(std::vector<T> &container, T &&item)
{
    auto hint = std::lower_bound(begin(container), end(container), item);

    if (hint == end(container) || item < *hint)
    {
        container.insert(hint, std::move(item));
        return true;
    }

    return false;
}
