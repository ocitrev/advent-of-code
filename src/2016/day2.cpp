#include "day2.hpp"
#include "../common.hpp"
#include <optional>

class Keypad
{
    int width_ = 0;
    int height_ = 0;
    std::vector<char> buttons_;

    Keypad(int width, int height)
        : width_{width}
        , height_{height}
        , buttons_(static_cast<size_t>(width * height), ' ')
    {
    }

public:
    static Keypad Parse(std::string_view text)
    {
        int height = 0;
        int width = 0;

        auto const lines = Split(text, '\n');

        for (auto line : lines)
        {
            int const len = static_cast<int>(line.size());
            width = std::max<int>(width, (len + len % 2) / 2);
            ++height;
        }

        Keypad keypad{width, height};
        int y = 0;

        for (auto line : lines)
        {
            int x = 0;

            for (auto iter = begin(line); iter < end(line);)
            {
                keypad.buttons_[static_cast<size_t>(y * width + x)] = *iter;
                ++x;

                if (++iter == end(line))
                    break;
                else
                    ++iter;
            }

            ++y;
        }

        return keypad;
    }

    std::optional<Point2d> FindPosition(char key) const
    {
        for (int y = 0; y != height_; ++y)
        {
            for (int x = 0; x != width_; ++x)
            {
                Point2d p{x, y};

                if (GetKey(p) == key)
                {
                    return p;
                }
            }
        }

        return std::nullopt;
    }

    char GetKey(Point2d pos) const
    {
        if (pos.x >= 0 && pos.y >= 0 && pos.x < width_ && pos.y < height_)
        {
            auto const offset = static_cast<size_t>(pos.y * width_ + pos.x);
            return buttons_.at(offset);
        }

        return ' ';
    }

    Point2d Move(Point2d cursor, Point2d dir) const
    {
        auto const newPos = cursor + dir;
        auto const candidate = GetKey(newPos);

        if (candidate == ' ')
            return cursor;

        return newPos;
    }
};

static Point2d ParseDir(char dir)
{
    switch (dir)
    {
    case 'U':
        return Point2d::NORTH;
    case 'D':
        return Point2d::SOUTH;
    case 'L':
        return Point2d::WEST;
    case 'R':
        return Point2d::EAST;
    }

    throw std::invalid_argument("dir");
}

static std::string ParseInstructions(std::string_view insctructions, Keypad const &keypad)
{
    std::string code;
    Point2d cursor = keypad.FindPosition('5').value();

    for (auto line : Split(insctructions, '\n'))
    {
        for (char d : line)
        {
            cursor = keypad.Move(cursor, ParseDir(d));
        }

        code.append(1, keypad.GetKey(cursor));
    }

    return code;
}

static std::string Part1()
{
    return ParseInstructions(input::instructions, Keypad::Parse(keypad1));
}

static std::string Part2()
{
    return ParseInstructions(input::instructions, Keypad::Parse(keypad2));
}

int main()
{
    // https://adventofcode.com/2016/day/2
    fmt::print("Day 2, 2016: Bathroom Security\n");

    Assert("1985" == ParseInstructions(example::instructions, Keypad::Parse(keypad1)));
    Assert("5DB3" == ParseInstructions(example::instructions, Keypad::Parse(keypad2)));

    auto const part1 = Part1();
    fmt::print("  Part 1: {}\n", part1);
    Assert("36629" == part1);

    auto const part2 = Part2();
    fmt::print("  Part 2: {}\n", part2);
    // Assert( == part2);
}