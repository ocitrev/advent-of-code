#include "day10.hpp"
#include "../common.hpp"
#include "../common/combinations.hpp"

struct Result
{
    int detected;
};

struct Ray
{
    double len;
    Point2d pos;

    Ray(double len_, Point2d pos_)
        : len(len_)
        , pos(pos_)
    {
    }
};

struct Asteroid
{
    Point2d pos;
    std::unordered_map<Point2d, std::vector<Ray>> rays;

    Asteroid(int x, int y)
        : pos{x, y}
    {
    }

    void AddRay(Asteroid const &other)
    {
        Point2d ray = other.pos - pos;
        double len = ray.Normalize();
        rays[ray].emplace_back(len, other.pos);
    }
};

[[nodiscard]] static std::vector<Asteroid> ParseMap(std::string_view map)
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

    ForEachCombinations(asteroids,
        [](Asteroid &a, Asteroid &b)
        {
            a.AddRay(b);
            b.AddRay(a);
        });

    return asteroids;
}

[[nodiscard]] static Asteroid GetAsteroidWithMostRays(std::vector<Asteroid> const &asteroids)
{
    auto maxIt = std::max_element(begin(asteroids), end(asteroids),
        [](Asteroid const &a, Asteroid const &b)
        {
            return a.rays.size() < b.rays.size();
        });

    Assert(maxIt != end(asteroids));

    return *maxIt;
}

[[maybe_unused]] static std::pair<int, Point2d> GetBestFromMap(std::string_view map)
{
    auto best = GetAsteroidWithMostRays(ParseMap(map));
    return {static_cast<int>(best.rays.size()), best.pos};
}

int main()
{
    fmt::print("Day 10, 2019: Monitoring Station\n");

    Assert(std::make_pair(8, Point2d{3, 4}) == GetBestFromMap(example::map1));
    Assert(std::make_pair(33, Point2d{5, 8}) == GetBestFromMap(example::map2));
    Assert(std::make_pair(35, Point2d{1, 2}) == GetBestFromMap(example::map3));
    Assert(std::make_pair(41, Point2d{6, 3}) == GetBestFromMap(example::map4));
    Assert(std::make_pair(210, Point2d{11, 13}) == GetBestFromMap(example::map5));

    auto map = ParseMap(GetInput());

    auto const best = GetAsteroidWithMostRays(map);
    fmt::print("  Part1: {}\n", best.rays.size());
    Assert(299 == best.rays.size());

    std::vector<std::pair<Point2d, Ray>> r;

    for (auto const &elem : best.rays)
    {
        r.emplace_back(elem.first, elem.second.front());
    }

    auto sortBySlope = [](auto const &a, auto const &b)
    {
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

    auto const el200 = begin(r) + 199;
    std::nth_element(begin(r), el200, end(r), sortBySlope);

    int const part2 = el200->second.pos.x * 100 + el200->second.pos.y;
    fmt::print("  Part2: {}\n", part2);
    Assert(1419 == part2);
}
