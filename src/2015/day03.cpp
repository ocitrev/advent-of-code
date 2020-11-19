#include "day03.hpp"
#include "../assert.hpp"
#include "../point.hpp"
#include <fmt/format.h>
#include <map>
#include <vector>

class HouseVisitor
{
    std::map<Point, int> visitedHouses;
    std::size_t currentPosition = 0;
    std::vector<Point> positions;

    Point Move(char direction)
    {
        Point &pos = positions[currentPosition];
        ++currentPosition;

        if (currentPosition == positions.size())
            currentPosition = 0;

        switch (direction)
        {
        case '>':
            ++pos.x;
            break;
        case '<':
            --pos.x;
            break;
        case '^':
            --pos.y;
            break;
        case 'v':
            ++pos.y;
            break;
        }

        return pos;
    }

public:
    explicit HouseVisitor(int playerCount = 1)
        : positions(static_cast<std::size_t>(playerCount))
    {
    }

    void Run(std::string_view path)
    {
        for (auto &p : positions)
            p = {};

        visitedHouses.clear();
        visitedHouses[Point{0, 0}] += static_cast<int>(positions.size());

        for (char direction : path)
        {
            ++visitedHouses[Move(direction)];
        }
    }

    [[nodiscard]] int GetVisitedHouses() const
    {
        return static_cast<int>(visitedHouses.size());
    }
};

int GetNbVisitedHouses(std::string_view path, int playerCount)
{
    HouseVisitor houseVisitor(playerCount);
    houseVisitor.Run(path);
    return houseVisitor.GetVisitedHouses();
}

int main()
{
    // https://adventofcode.com/2015/day/3
    fmt::print("Day 3: Perfectly Spherical Houses in a Vacuum\n");

    Assert(2 == GetNbVisitedHouses(">"));
    Assert(4 == GetNbVisitedHouses("^>v<"));
    Assert(2 == GetNbVisitedHouses("^v^v^v^v^v"));

    fmt::print("  Part 1: {}\n", GetNbVisitedHouses(input::data, 1));
    fmt::print("  Part 2: {}\n", GetNbVisitedHouses(input::data, 2));

    for (int i = 3; i < 100; ++i)
    {
        fmt::print("  Part {}: {}\n", i, GetNbVisitedHouses(input::data, i));
    }
}
