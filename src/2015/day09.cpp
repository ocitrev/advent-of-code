#include "day09.hpp"
#include "../common/assert.hpp"
#include "../common/string.hpp"
#include <fmt/format.h>
#include <set>

struct Route
{
    std::string a;
    std::string b;
    int distance = 0;

    explicit Route(std::string_view line)
    {
        auto parts = Split(line, '=');
        auto &names = parts[0];
        auto distanceText = parts[1];
        trim(names);
        trim(distanceText);
        distance = std::stoi(distanceText);
        auto pos = names.find(" to ");
        a = names.substr(0, pos);
        b = names.substr(pos + 4, names.size());

        if (b < a)
            std::swap(a, b);
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
    std::vector<Route> routes;
    auto lines = Split(data, '\n');
    routes.reserve(lines.size());
    std::set<std::string> citiySet;

    for (auto &&line : lines)
    {
        Route &r = routes.emplace_back(line);
        // fmt::print("{} -> {} = {}\n", r.a, r.b, r.distance);
        citiySet.insert(r.a);
        citiySet.insert(r.b);
    }

    std::vector<std::string> cities(begin(citiySet), end(citiySet));
    std::sort(begin(cities), end(cities));
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

        // for (auto &&city : cities)
        //     fmt::print("{} -> ", city);
        // fmt::print("{}\n", total);
    } while (std::next_permutation(begin(cities), end(cities)));

    return best;
}

int main()
{
    // https://adventofcode.com/2015/day/9
    fmt::print("Day 9: All in a Single Night\n");

    Assert(605 == FindShortest(input::example, std::less<>{}));
    Assert(982 == FindShortest(input::example, std::greater<>{}));

    fmt::print("  Part 1: {}\n", FindShortest(input::data, std::less<>{}));
    fmt::print("  Part 2: {}\n", FindShortest(input::data, std::greater<>{}));
}
