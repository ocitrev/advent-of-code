#pragma once

#include <vector>
#include <filesystem>
#include <cctype>


[[nodiscard]]
std::filesystem::path GetInputsPath();

[[nodiscard]]
std::vector<int> ReadInt32s(std::filesystem::path const &filepath, char sep);

[[nodiscard]]
std::vector<long long> ReadInt64s(std::filesystem::path const &filepath, char sep);

[[nodiscard]]
std::string ReadAllText(std::filesystem::path const &filepath);

// trim from start (in place)
inline void ltrim(std::string &s)
{
    auto is_not_space = [](unsigned char ch)
    {
        return !std::isspace(ch);
    };

    s.erase(begin(s), std::find_if(begin(s), end(s), is_not_space));
}

// trim from end (in place)
inline void rtrim(std::string &s)
{
    auto is_not_space = [](unsigned char ch)
    {
        return !std::isspace(ch);
    };

    s.erase(std::find_if(rbegin(s), rend(s), is_not_space).base(), end(s));
}

// trim from both ends (in place)
inline void trim(std::string &s)
{
    ltrim(s);
    rtrim(s);
}
