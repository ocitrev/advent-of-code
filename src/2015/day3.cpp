#include "day3.hpp"

#include "../common/assert.hpp"
#include "../common/point2d.hpp"

#include <fmt/format.h>
#include <map>
#include <vector>

class HouseVisitor
{
    std::map<Point2d, int> visitedHouses;
    std::size_t currentPosition = 0;
    std::vector<Point2d> positions;

    Point2d Move(char direction)
    {
        Point2d &pos = positions[currentPosition];
        ++currentPosition;

        if (currentPosition == positions.size())
        {
            currentPosition = 0;
        }

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
        {
            p = {};
        }

        visitedHouses.clear();
        visitedHouses[Point2d{0, 0}] += static_cast<int>(positions.size());

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

static int GetNbVisitedHouses(std::string_view path, int playerCount)
{
    HouseVisitor houseVisitor(playerCount);
    houseVisitor.Run(path);
    return houseVisitor.GetVisitedHouses();
}

int main()
{
    // https://adventofcode.com/2015/day/3
    fmt::print("Day 3, 2015: Perfectly Spherical Houses in a Vacuum\n");

    Assert(2 == GetNbVisitedHouses(">", 1));
    Assert(4 == GetNbVisitedHouses("^>v<", 1));
    Assert(2 == GetNbVisitedHouses("^v^v^v^v^v", 1));

    int const part1 = GetNbVisitedHouses(GetInput(), 1);
    fmt::print("  Part 1: {}\n", part1);
    Assert(2572 == part1);

    int const part2 = GetNbVisitedHouses(GetInput(), 2);
    fmt::print("  Part 2: {}\n", part2);
    Assert(2631 == part2);
}
