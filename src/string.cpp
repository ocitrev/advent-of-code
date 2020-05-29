#include "string.hpp"

std::vector<std::string> Split(std::string_view text, char sep)
{
    std::vector<std::string> result;
    std::string_view::const_iterator first = begin(text);
    std::string_view::const_iterator iter = first;
    std::string_view::const_iterator last = end(text);

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
