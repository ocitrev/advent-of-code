#include "day14.hpp"

#include "../common.hpp"

struct Line
{
    Point2d start;
    Point2d end;
};

static constexpr Point2d const dropLocation{500, 0};

struct Map
{
    std::unordered_map<Point2d, char> grid;
    int floorHeight = 0;
    Point2d min = dropLocation;
    Point2d max = dropLocation;

    Map()
    {
        grid[dropLocation] = Crack;
    }

    static constexpr char Air = '.';
    static constexpr char Rock = '#';
    static constexpr char Crack = '+';
    static constexpr char Sand = 'o';

    static Point2d ParsePoint(std::string_view point)
    {
        auto const parts = Split(point, ',');
        return {svtoi(parts[0]), svtoi(parts[1])};
    }

    static std::vector<Point2d> ParseScanLine(std::string_view text)
    {
        auto coords = Split(text, " -> ");
        std::vector<Point2d> points(coords.size());
        std::transform(begin(coords), end(coords), begin(points), &ParsePoint);
        return points;
    }

    void ParseScanLines(std::string_view scanlines)
    {
        for (auto line : Split(scanlines, '\n'))
        {
            if (line.empty())
            {
                continue;
            }

            auto const points = ParseScanLine(line);
            AddLineStrip(points);
        }
    }

    void AddLineStrip(std::vector<Point2d> const &points)
    {
        auto prev = begin(points);
        auto last = end(points);

        for (auto iter = prev + 1; iter != last; ++prev, ++iter)
        {
            AddWall(Line{*prev, *iter});
        }
    }

    void AddWall(Line const &wall)
    {
        if (wall.start.x == wall.end.x)
        {
            // vertical
            auto const [startY, endY] = std::minmax(wall.start.y, wall.end.y);

            for (int y = startY; y <= endY; ++y)
            {
                SetCell({wall.start.x, y}, Rock);
            }
        }
        else if (wall.start.y == wall.end.y)
        {
            // horizontal
            auto const [startX, endX] = std::minmax(wall.start.x, wall.end.x);

            for (int x = startX; x <= endX; ++x)
            {
                SetCell({x, wall.start.y}, Rock);
            }
        }
        else
        {
            throw std::invalid_argument("wall");
        }
    }

    void SetCell(Point2d p, char value)
    {
        grid[p] = value;

        min.x = std::min(min.x, p.x);
        min.y = std::min(min.y, p.y);
        max.x = std::max(max.x, p.x);
        max.y = std::max(max.y, p.y);
    }

    char GetCell(Point2d pos) const
    {
        if (auto iter = grid.find(pos); iter != end(grid))
        {
            return iter->second;
        }

        return Air;
    }

    void Draw() const
    {
        int const w = max.x - min.x + 1;
        int const h = max.y - min.y + 1;

        for (int y = 0; y != h; ++y)
        {
            for (int x = 0; x != w; ++x)
            {
                std::print("{}", GetCell({x + min.x, y + min.y}));
            }

            std::print("\n");
        }
    }

    enum struct State
    {
        Fall,
        Left,
        Right,
        Stop,
        Drop,
    };

    State Fall(Point2d &sand)
    {
        Point2d const candidate = sand + Point2d::SOUTH;

        if (floorHeight != 0 && candidate.y == floorHeight)
        {
            return State::Stop;
        }

        if (candidate.y > max.y + 2)
        {
            return State::Drop;
        }

        if (GetCell(candidate) == Air)
        {
            sand = candidate;
            return State::Fall;
        }

        return State::Left;
    }

    State Left(Point2d &sand)
    {
        Point2d const candidate = sand + Point2d::SOUTH_WEST;

        if (GetCell(candidate) == Air)
        {
            sand = candidate;
            return State::Fall;
        }

        return State::Right;
    }

    State Right(Point2d &sand)
    {
        Point2d const candidate = sand + Point2d::SOUTH_EAST;

        if (GetCell(candidate) == Air)
        {
            sand = candidate;
            return State::Fall;
        }

        return State::Stop;
    }

    bool StepSimulation()
    {
        Point2d sand = dropLocation;
        State state = State::Fall;

        while (true)
        {
            switch (state)
            {
                case State::Fall:
                    state = Fall(sand);
                    break;

                case State::Left:
                    state = Left(sand);
                    break;

                case State::Right:
                    state = Right(sand);
                    break;

                case State::Stop:
                    SetCell(sand, Sand);
                    return sand != dropLocation;

                case State::Drop:
                    return false;
            }
        }
    }

    void AddFloor()
    {
        floorHeight = max.y + 2;
    }
};

static int Simulate(std::string_view scanlines, bool withFloor)
{
    Map map;
    map.ParseScanLines(scanlines);

    if (withFloor)
    {
        map.AddFloor();
    }

    int count = withFloor ? 1 : 0;

    while (map.StepSimulation())
    {
        ++count;
    }

    // map.Draw();
    return count;
}

static auto Part1()
{
    return Simulate(GetInput(), false);
}

static auto Part2()
{
    return Simulate(GetInput(), true);
}

int main()
{
    // https://adventofcode.com/2022/day/14
    std::print("Day 14, 2022: Regolith Reservoir\n");

    Assert(24 == Simulate(example::scanlines, false));
    Assert(93 == Simulate(example::scanlines, true));

    auto const part1 = Part1();
    std::print("  Part 1: {}\n", part1);
    Assert(728 == part1);

    auto const part2 = Part2();
    std::print("  Part 2: {}\n", part2);
    // Assert( == part2);
}
