#include "string.hpp"

std::vector<std::string_view> Split(std::string_view text, std::string_view sep)
{
    std::vector<std::string_view> result;
    std::size_t offset = 0;
    auto pos = text.find(sep, offset);

    while (pos != std::string_view::npos)
    {
        result.emplace_back(text.substr(offset, pos - offset));
        offset = pos + sep.size();
        pos = text.find(sep, offset);
    }

    result.emplace_back(text.substr(offset));
    return result;
}

std::vector<std::string_view> SplitFirstOf(std::string_view text, std::string_view sep)
{
    std::vector<std::string_view> result;
    std::size_t offset = 0;
    auto pos = text.find_first_of(sep, offset);

    while (pos != std::string_view::npos)
    {
        result.emplace_back(text.substr(offset, pos - offset));
        offset = text.find_first_not_of(sep, pos);
        pos = text.find_first_of(sep, offset);
    }

    result.emplace_back(text.substr(offset));
    return result;
}
