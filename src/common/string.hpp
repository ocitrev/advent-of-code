#pragma once
#include <cctype>
#include <charconv>
#include <functional>
#include <locale>
#include <numeric>
#include <string>
#include <string_view>
#include <vector>

#if __has_include(<version>)
#    include <version>
#endif

[[nodiscard]] std::vector<std::string_view> Split(std::string_view text, std::string_view sep);
[[nodiscard]] std::vector<std::string_view> SplitFirstOf(std::string_view text, std::string_view sep);

[[nodiscard]] inline std::vector<std::string_view> Split(std::string_view text, char sep = '\n')
{
    return Split(text, std::string_view{&sep, 1});
}

template <class T, class BinaryReductionOp, class UnaryTransformOp>
[[nodiscard]] inline auto TransformReduceLines(
    std::string_view text, T &&init, BinaryReductionOp &&r, UnaryTransformOp &&t)
{
    auto const lines = Split(text, '\n');
    return std::transform_reduce(begin(lines), end(lines), std::forward<T>(init), std::forward<BinaryReductionOp>(r),
        std::forward<UnaryTransformOp>(t));
}

template <typename IterT>
[[nodiscard]] inline std::string Join(IterT first, IterT last, std::string_view sep)
{
    std::string result;

    if (first == last)
        return result;

    for (auto iter = first; iter != last; ++iter)
    {
        if (iter != first)
            result += sep;

        result += *iter;
    }

    return result;
}

template <typename IterT, typename ToStringT>
[[nodiscard]] inline std::string Join(IterT first, IterT last, std::string_view sep, ToStringT &&toString)
{
    std::string result;

    if (first == last)
        return result;

    for (auto iter = first; iter != last; ++iter)
    {
        if (iter != first)
            result += sep;

        result += toString(*iter);
    }

    return result;
}

// trim from start (in place)
template <typename PredicateT>
inline void ltrim_if(std::string &s, PredicateT &&pred)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not_fn(std::forward<PredicateT>(pred))));
}

// trim from start (in place)
inline void ltrim(std::string &s)
{
    return ltrim_if(s, &isspace);
}

// trim from start (in place)
template <typename PredicateT>
inline void ltrim_if(std::string_view &s, PredicateT &&pred)
{
    s.remove_prefix(static_cast<std::size_t>(
        std::distance(s.begin(), std::find_if(s.begin(), s.end(), std::not_fn(std::forward<PredicateT>(pred))))));
}

// trim from start (in place)
inline void ltrim(std::string_view &s)
{
    ltrim_if(s, &isspace);
}

// trim from end (in place)
template <typename PredicateT>
inline void rtrim_if(std::string &s, PredicateT &&pred)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not_fn(std::forward<PredicateT>(pred))).base(), s.end());
}

// trim from end (in place)
inline void rtrim(std::string &s)
{
    rtrim_if(s, &isspace);
}

// trim from end (in place)
template <typename PredicateT>
inline void rtrim_if(std::string_view &s, PredicateT &&pred)
{
    s.remove_suffix(static_cast<std::size_t>(
        std::distance(s.rbegin(), std::find_if(s.rbegin(), s.rend(), std::not_fn(std::forward<PredicateT>(pred))))));
}

// trim from end (in place)
inline void rtrim(std::string_view &s)
{
    return rtrim_if(s, &isspace);
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

[[nodiscard]] inline std::string_view trim_copy(std::string_view s)
{
    trim(s);
    return s;
}

// trim from start (copying)
[[nodiscard]] inline std::string ltrim_copy(std::string s)
{
    ltrim(s);
    return s;
}

// trim from end (copying)
[[nodiscard]] inline std::string rtrim_copy(std::string s)
{
    rtrim(s);
    return s;
}

// trim from end (copying)
template <typename PredicateT>
[[nodiscard]] inline std::string rtrim_copy_if(std::string s, PredicateT &&pred)
{
    rtrim_if(s, pred);
    return s;
}

template <typename IntT = int>
[[nodiscard]] inline IntT svtoi(std::string_view text)
{
    IntT result = 0;
    // NOLINTNEXTLINE cppcoreguidelines-pro-bounds-pointer-arithmetic
    std::from_chars(text.data(), text.data() + text.size(), result);
    return result;
}

template <typename IntT = int>
[[nodiscard]] inline IntT svtoi(std::string_view text, int base)
{
    IntT result = 0;
    // NOLINTNEXTLINE cppcoreguidelines-pro-bounds-pointer-arithmetic
    std::from_chars(text.data(), text.data() + text.size(), result, base);
    return result;
}

#if __cpp_lib_starts_ends_with >= 201711
[[nodiscard]] inline constexpr bool starts_with(std::string_view text, std::string_view prefix)
{
    return text.starts_with(prefix);
}
#else
[[nodiscard]] inline constexpr bool starts_with(std::string_view text, std::string_view prefix)
{
    return text.size() >= prefix.size() && text.compare(0, prefix.size(), prefix) == 0;
}
#endif

#if __cpp_lib_starts_ends_with >= 201711
[[nodiscard]] inline constexpr bool ends_with(std::string_view text, std::string_view prefix)
{
    return text.ends_with(prefix);
}
#else
[[nodiscard]] inline constexpr bool ends_with(std::string_view text, std::string_view suffix)
{
    return suffix.size() <= text.size() && text.compare(text.size() - suffix.size(), suffix.size(), suffix) == 0;
}
#endif

[[nodiscard]] inline constexpr std::string_view NextLine(std::string_view text)
{
    if (auto const pos = text.find('\n'); pos != text.npos)
    {
        return text.substr(pos + 1);
    }

    return text;
}
