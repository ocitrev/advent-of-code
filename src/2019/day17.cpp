#include "day17.hpp"
#include "../common/assert.hpp"
#include "../common/intcode.hpp"
#include "../common/terminal.hpp"
#include <array>
#include <fmt/format.h>
#include <string>
#include <string_view>
#include <thread>

using Map = std::pair<std::string, Int>;

Int GetMapWidth(std::string_view map)
{
    return std::distance(begin(map), std::find(begin(map), end(map), '\n'));
}

void CleanMap(Map &map)
{
    map.first.erase(std::remove(begin(map.first), end(map.first), '\n'), end(map.first));
}

Map GenerateMap()
{
    Map ret;

    auto out = [&](Int value) {
        ret.first.append(1, static_cast<char>(value));
    };

    Intcode::Run(input::data, nullptr, out);
    ret.second = GetMapWidth(ret.first);
    CleanMap(ret);
    return ret;
}

struct Pos
{
    Int x = 0;
    Int y = 0;

    constexpr Pos() = default;

    constexpr Pos(Int x_, Int y_)
        : x{x_}
        , y{y_}
    {
    }

    constexpr Pos(std::lldiv_t d)
        : x{d.rem}
        , y{d.quot}
    {
    }

    constexpr Pos operator+(Pos const &p) const
    {
        return {x + p.x, y + p.y};
    }

    constexpr Pos &operator+=(Pos const &p)
    {
        x += p.x;
        y += p.y;
        return *this;
    }

    constexpr Pos operator-(Pos const &p) const
    {
        return {x - p.x, y - p.y};
    }

    constexpr Pos &operator-=(Pos const &p)
    {
        x -= p.x;
        y -= p.y;
        return *this;
    }

    constexpr bool operator==(Pos const &p) const
    {
        return x == p.x && y == p.y;
    }
};

constexpr static Pos West{-1, 0};
constexpr static Pos East{+1, 0};
constexpr static Pos North{0, -1};
constexpr static Pos South{0, +1};

Int GetMapHeight(Map const &map)
{
    return static_cast<Int>(map.first.size()) / map.second;
}

char GetTile(Map const &map, Pos const &p)
{
    if (p.x < 0 || p.y < 0 || p.x >= map.second)
        return '\0';

    Int const height = GetMapHeight(map);

    if (p.y >= height)
        return '\0';

    return map.first[static_cast<size_t>(p.y * map.second + p.x)];
}

Int MarkIntersections(Map const &map)
{
    Int result = 0;
    Int const height = static_cast<Int>(map.first.size()) / map.second;
    Pos p;

    for (char const &c : map.first)
    {
        if (p.y > 0 && c == '#' && p.x > 0 && p.x < map.second - Int{1} && p.y < height - Int{1})
        {
            char l = GetTile(map, p + West);
            char r = GetTile(map, p + East);
            char u = GetTile(map, p + North);
            char d = GetTile(map, p + South);

            if (l == '#' && r == '#' && u == '#' && d == '#')
            {
                result += p.x * p.y;
            }
        }

        if (++p.x == map.second)
        {
            ++p.y;
            p.x = 0;
        }
    }

    return result;
}

void RenderMap(Map const &map)
{
    std::string line(static_cast<size_t>(map.second), '\0');
    auto iter = begin(line);

    for (char const &c : map.first)
    {
        *iter = c;

        if (++iter == end(line))
        {
            fmt::print("{}\n", line);
            iter = begin(line);
        }
    }
}

static Pos GetDir(char robot)
{
    switch (robot)
    {
    case '<':
        return West;
    case '>':
        return East;
    case '^':
        return North;
    case 'v':
        return South;
    }

    throw std::invalid_argument{"Invalid robot"};
}

static std::array<std::pair<Pos, char>, 2> GetOtherDirs(Pos const &pos)
{
    if (pos == West)
        return {std::make_pair(South, 'L'), std::make_pair(North, 'R')};

    if (pos == East)
        return {std::make_pair(North, 'L'), std::make_pair(South, 'R')};

    if (pos == North)
        return {std::make_pair(West, 'L'), std::make_pair(East, 'R')};

    if (pos == South)
        return {std::make_pair(East, 'L'), std::make_pair(West, 'R')};

    throw std::invalid_argument{"Invalid robot"};
}

void FillPath(std::string &path, Map const &map, Pos start, Pos dir)
{
    while (true)
    {
        bool hasPath = false;

        for (auto const &d : GetOtherDirs(dir))
        {
            if (GetTile(map, start + d.first) == '#')
            {
                path.append(1, ',');
                path.append(1, d.second);
                hasPath = true;
                dir = d.first;
                break;
            }
        }

        if (hasPath)
        {
            int count = 0;

            while (GetTile(map, start + dir) == '#')
            {
                start += dir;
                ++count;
            }

            path.append(1, ',');
            path.append(std::to_string(count));
        }
        else
        {
            break;
        }
    }
}

std::string GetPath(Map const &map)
{
    using namespace std::string_view_literals;
    auto pos = map.first.find('^');
    Pos start = std::lldiv(static_cast<Int>(pos), map.second);

    std::string path;
    FillPath(path, map, start, GetDir('^'));
    path.erase(begin(path));
    return path;
}

void Part1()
{
    auto m = GenerateMap();
    auto const part1 = MarkIntersections(m);
    fmt::print("  Part1: {}\n", part1);
    Assert(14332 == part1);
}

void Part2()
{
    constexpr bool debug = false;

    Intcode cpu(input::data);
    cpu.WriteMemory(0, 2);
    Int result = 0;

    if constexpr (debug)
    {
        auto m = GenerateMap();
        std::string path = GetPath(m);

        std::string ret;
        Int prev = 0;

        auto outDebug = [&](Int value) {
            if (value > 0x7f)
            {
                result = value;
            }
            else
            {
                ret.append(1, static_cast<char>(value));

                if (value == '\n' && prev == '\n')
                {
                    fwrite("\x1b[f", 1, 4, stdout);
                    fwrite(ret.data(), 1, ret.size(), stdout);
                    ret.clear();
                    using namespace std::chrono_literals;
                    std::this_thread::sleep_for(50ms);
                }
            }

            prev = value;
        };

        cpu.SetOutput(outDebug);
    }
    else
    {
        cpu.SetOutput([&](Int value) {
            result = value;
        });
    }

    auto sendInputs = [&cpu](std::string sequence) {
        while (!sequence.empty())
        {
            cpu.RunUntilInput()(sequence.front());
            sequence.erase(begin(sequence));
        }
    };

    sendInputs("A,B,A,B,C,C,B,C,B,A\n");
    sendInputs("R,12,L,8,R,12\n");
    sendInputs("R,8,R,6,R,6,R,8\n");
    sendInputs("R,8,L,8,R,8,R,4,R,4\n");
    sendInputs("n\n");
    cpu.Run();

    fmt::print("  Part2: {}\n", result);
    Assert(1034009 == result);
}

Map ParseMap(std::string_view mapData)
{
    auto map = std::make_pair(std::string{mapData.data(), mapData.size()}, GetMapWidth(mapData));
    CleanMap(map);
    return map;
}

int main()
{
    fmt::print("Day 17, 2019: Set and Forget\n");
    Assert(13 == GetMapWidth(example::map1));
    Assert(76 == MarkIntersections(ParseMap(example::map1)));
    Assert("R,8,R,8,R,4,R,4,R,8,L,6,L,2,R,4,R,4,R,8,R,8,R,8,L,6,L,2" == GetPath(ParseMap(example::map2)));

    Part1();
    Part2();
}
