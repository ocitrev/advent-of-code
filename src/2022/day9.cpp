#include "day9.hpp"

#include "../common.hpp"

struct Instruction
{
    Point2d dir;
    int len;

    static Instruction Parse(char dir, int len)
    {
        switch (dir)
        {
            case 'L':
                return {Point2d::WEST, len};
            case 'U':
                return {Point2d::NORTH, len};
            case 'R':
                return {Point2d::EAST, len};
            case 'D':
                return {Point2d::SOUTH, len};
        }

        return {};
    }
};

static std::vector<Instruction> ParseInstructions(std::string_view text)
{
    std::vector<Instruction> instructions;

    for (auto line : Split(text, '\n'))
    {
        char const dir = line[0];
        int len = svtoi(line.substr(2));
        instructions.push_back(Instruction::Parse(dir, len));
    }

    return instructions;
}

struct Map
{
    std::unordered_map<Point2d, char> grid;
    std::vector<Point2d> rope;

    Map(size_t knots)
        : rope(knots)
    {
        grid[rope.back()] = '#';
    }

    void Run(std::string_view instructions)
    {
        Run(ParseInstructions(instructions));
    }

    void Run(std::vector<Instruction> const &instructions)
    {
        for (auto const &ins : instructions)
        {
            for (int i = 0; i != ins.len; ++i)
            {
                Update(ins.dir);
            }
        }
    }

    void Update(Point2d dir)
    {
        auto const last = end(rope);
        auto prev = begin(rope);
        *prev += dir;

        for (auto iter = std::next(prev); iter != last; ++iter, ++prev)
        {
            auto const delta = (*iter - *prev) / 2;

            if (delta == Point2d{})
            {
                continue;
            }

            *iter = *prev + delta;
        }

        grid[rope.back()] = '#';
    }

    void Print() const
    {
        int minX = std::numeric_limits<int>::max();
        int maxX = std::numeric_limits<int>::min();
        int minY = std::numeric_limits<int>::max();
        int maxY = std::numeric_limits<int>::min();

        for (auto const &[p, _] : grid)
        {
            minX = std::min(p.x, minX);
            maxX = std::max(p.x, maxX);
            minY = std::min(p.y, minY);
            maxY = std::max(p.y, maxY);
        }

        int const w = maxX - minX + 1;
        int const h = maxY - minY + 1;

        for (int y = 0; y != h; ++y)
        {
            for (int x = 0; x != w; ++x)
            {
                Point2d p{x + minX, y + minY};

                if (auto iter = grid.find(p); iter != end(grid))
                {
                    std::print("{}", iter->second);
                }
                else
                {
                    std::print(" ");
                }
            }

            std::print("\n");
        }
    }

    int CountVisited() const
    {
        int count = 0;

        for (auto const &[_, cell] : grid)
        {
            if (cell == '#')
            {
                ++count;
            }
        }

        return count;
    }
};

static int CountVisited(std::string_view text, size_t knots)
{
    Map map{knots};
    map.Run(text);
    return map.CountVisited();
}

static int Part1()
{
    return CountVisited(GetInput(), 2);
}

static int Part2()
{
    return CountVisited(GetInput(), 10);
}

int main()
{
    // https://adventofcode.com/2022/day/9
    std::print("Day 9, 2022: Rope Bridge\n");

    Assert(13 == CountVisited(example::instructions, 2));
    Assert(1 == CountVisited(example::instructions, 10));
    Assert(36 == CountVisited(example::larger, 10));

    auto const part1 = Part1();
    std::print("  Part 1: {}\n", part1);
    Assert(5513 == part1);

    auto const part2 = Part2();
    std::print("  Part 2: {}\n", part2);
    Assert(2427 == part2);
}
