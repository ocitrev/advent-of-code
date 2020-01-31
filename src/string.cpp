#include "string.hpp"

std::vector<std::string> Split(std::string_view text, char sep)
{
    std::vector<std::string> result;

    auto first = begin(text);
    auto iter = first;
    auto const last = end(text);

    for (; iter != last; ++iter)
    {
        if (*iter != sep)
            continue;

        result.emplace_back(first, iter);
        first = iter + 1;
    }

    result.emplace_back(first, iter);

    return result;
}
