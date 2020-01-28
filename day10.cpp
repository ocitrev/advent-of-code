#include "day10.hpp"
#include "common/combinations.hpp"
#include "common/main.hpp"
#include "common/point.hpp"
#include "common/string.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <map>
#include <vector>

struct Result
{
    int detected;
};

struct Ray
{
    double len;
    Point pos;

    Ray(double len, Point pos)
        : len(len)
        , pos(pos)
    {
    }
};

struct Asteroid
{
    Point pos;
    std::map<Point, std::vector<Ray>> rays;

    Asteroid(int x, int y)
        : pos{x, y}
    {
    }

    void AddRay(Asteroid const &other)
    {
        Point ray = other.pos - pos;
        double len = ray.Normalize();
        rays[ray].emplace_back(len, other.pos);
    }
};

[[nodiscard]] std::vector<Asteroid> ParseMap(std::string_view map)
{
    auto lines = Split(map, '\n');
    std::vector<Asteroid> asteroids;
    int y = 0;

    for (auto const &line : lines)
    {
        int x = 0;

        for (auto c : line)
        {
            if (c == '#')
            {
                asteroids.emplace_back(x, y);
            }

            ++x;
        }

        ++y;
    }

    ForEachCombinations(asteroids, [](Asteroid &a, Asteroid &b) {
        a.AddRay(b);
        b.AddRay(a);
    });

    return asteroids;
}

[[nodiscard]] Asteroid GetAsteroidWithMostRays(std::vector<Asteroid> const &asteroids)
{
    auto maxIt = std::max_element(begin(asteroids), end(asteroids),
                                  [](Asteroid const &a, Asteroid const &b) {
                                      return a.rays.size() < b.rays.size();
                                  });

    assert(maxIt != end(asteroids));

    return *maxIt;
}

std::pair<int, Point> GetBestFromMap(std::string_view map)
{
    auto best = GetAsteroidWithMostRays(ParseMap(map));
    return {static_cast<int>(best.rays.size()), best.pos};
}

int main()
{
    std::cout << "Day 10: Monitoring Station\n";

    assert(std::make_pair(8, Point{3, 4}) == GetBestFromMap(example::map1));
    assert(std::make_pair(33, Point{5, 8}) == GetBestFromMap(example::map2));
    assert(std::make_pair(35, Point{1, 2}) == GetBestFromMap(example::map3));
    assert(std::make_pair(41, Point{6, 3}) == GetBestFromMap(example::map4));
    assert(std::make_pair(210, Point{11, 13}) == GetBestFromMap(example::map5));

    auto map = ParseMap(input::map);

    auto best = GetAsteroidWithMostRays(map);
    std::cout << "  Part1: " << best.rays.size() << '\n';
    assert(299 == best.rays.size());

    std::vector<std::pair<Point, Ray>> r;

    for (auto const &elem : best.rays)
    {
        r.emplace_back(elem.first, elem.second.front());
    }

    auto sortBySlope = [](auto const &a, auto const &b) {
        double slopeA = a.first.Slope() + M_PI_2;
        double slopeB = b.first.Slope() + M_PI_2;

        if (slopeA < 0)
        {
            slopeA += M_PI * 2.0;
        }

        if (slopeB < 0)
        {
            slopeB += M_PI * 2.0;
        }

        return slopeA < slopeB;
    };

    auto el200 = begin(r) + 199;
    std::nth_element(begin(r), el200, end(r), sortBySlope);

    int part2 = el200->second.pos.x * 100 + el200->second.pos.y;
    std::cout << "  Part2: " << part2 << '\n';
    assert(1419 == part2);
}
