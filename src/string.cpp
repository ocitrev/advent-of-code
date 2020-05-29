#include "string.hpp"

std::vector<std::string> Split(std::string_view text, char sep)
{
    std::vector<std::string> result;

    auto first = begin(text);
    auto iter = first;
    auto last = end(text);

    for (; iter != last; std::advance(iter, 1))
    {
        if (*iter != sep)
            continue;

        result.emplace_back(first, iter);
        first = std::next(iter);
    }

    result.emplace_back(first, iter);

    return result;
}
