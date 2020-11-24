#pragma once
#include <algorithm>
#include <cctype>
#include <charconv>
#include <locale>
#include <string>
#include <string_view>
#include <vector>

[[nodiscard]] std::vector<std::string> Split(std::string_view text, char sep);

// trim from start (in place)
inline void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
                return std::isspace(ch) == 0;
            }));
}

// trim from start (in place)
inline void ltrim(std::string_view &s)
{
    s.remove_prefix(static_cast<std::size_t>(std::distance(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
                                                               return std::isspace(ch) == 0;
                                                           }))));
}

// trim from end (in place)
inline void rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         [](int ch) {
                             return std::isspace(ch) == 0;
                         })
                .base(),
            s.end());
}

// trim from end (in place)
inline void rtrim(std::string_view &s)
{
    s.remove_suffix(static_cast<std::size_t>(std::distance(s.rbegin(), std::find_if(s.rbegin(), s.rend(), [](int ch) {
                                                               return std::isspace(ch) == 0;
                                                           }))));
}

// trim from both ends (in place)
inline void trim(std::string &s)
{
    ltrim(s);
    rtrim(s);
}

// trim from both ends (in place)
inline void trim(std::string_view &s)
{
    ltrim(s);
    rtrim(s);
}

// trim from start (copying)
inline std::string ltrim_copy(std::string s)
{
    ltrim(s);
    return s;
}

// trim from end (copying)
inline std::string rtrim_copy(std::string s)
{
    rtrim(s);
    return s;
}

inline int svtoi(std::string_view text)
{
    int result = 0;
    // NOLINTNEXTLINE cppcoreguidelines-pro-bounds-pointer-arithmetic
    std::from_chars(text.data(), text.data() + text.size(), result);
    return result;
}
