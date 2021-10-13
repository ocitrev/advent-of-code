#include "day09.hpp"
#include "../common/assert.hpp"
#include "../common/string.hpp"
#include "../common/utils.hpp"
#include <fmt/format.h>

struct Route
{
    std::string a;
    std::string b;
    int distance = 0;

    static Route Parse(std::string_view line)
    {
        auto parts = Split(line, '=');
        auto &names = parts[0];
        auto distanceText = parts[1];
        trim(names);
        trim(distanceText);
        int distance = svtoi(distanceText);
        auto pos = names.find(" to ");
        auto a = names.substr(0, pos);
        auto b = names.substr(pos + 4, names.size());

        if (b < a)
            std::swap(a, b);

        return {std::string{a}, std::string{b}, distance};
    }

    bool operator==(Route const &other) const
    {
        return std::tie(a, b, distance) == std::tie(other.a, other.b, other.distance);
    }

    bool operator!=(Route const &other) const
    {
        return std::tie(a, b, distance) != std::tie(other.a, other.b, other.distance);
    }

    bool operator<(Route const &other) const
    {
        return std::tie(a, b, distance) < std::tie(other.a, other.b, other.distance);
    }
};

int FindRoute(std::vector<Route> const &routes, std::string_view a, std::string_view b)
{
    auto iter = std::find_if(begin(routes), end(routes), [&](Route const &r) {
        return (r.a == a && r.b == b) || (r.a == b && r.b == a);
    });

    if (iter != end(routes))
        return iter->distance;

    return -1;
}

template <typename PredicateT>
int FindShortest(std::string_view data, PredicateT &&pred)
{
    auto lines = Split(data, '\n');
    std::vector<Route> routes;
    routes.reserve(lines.size());
    std::vector<std::string> cities;
    cities.reserve(lines.size() * 2);

    for (auto &&line : lines)
    {
        Route &r = routes.emplace_back(Route::Parse(line));
#if DEBUG_PRINT
        fmt::print("{} -> {} = {}\n", r.a, r.b, r.distance);
#endif
        insert_sorted(cities, r.a);
        insert_sorted(cities, r.b);
    }

    int best = -1;

    do
    {
        auto prev = begin(cities);
        int total = 0;

        for (auto iter = std::next(prev); iter != end(cities); ++iter, ++prev)
        {
            int const dist = FindRoute(routes, *prev, *iter);
            Assert(dist != -1);
            total += dist;
        }

        if (best == -1 || pred(total, best))
            best = total;

#if DEBUG_PRINT
        for (auto &&city : cities)
            fmt::print("{} -> ", city);
        fmt::print("{}\n", total);
#endif
    } while (std::next_permutation(begin(cities), end(cities)));

    return best;
}

int main()
{
    // https://adventofcode.com/2015/day/9
    fmt::print("Day 9, 2015: All in a Single Night\n");

    Assert(605 == FindShortest(input::example, std::less<>{}));
    Assert(982 == FindShortest(input::example, std::greater<>{}));

    int const part1 = FindShortest(input::data, std::less<>{});
    fmt::print("  Part 1: {}\n", part1);
    Assert(117 == part1);

    int const part2 = FindShortest(input::data, std::greater<>{});
    fmt::print("  Part 2: {}\n", part2);
    Assert(909 == part2);
}
