#include "day4.hpp"
#include "../common.hpp"

static std::string_view pop_back(std::string_view text)
{
    text.remove_suffix(1);
    return text;
}

struct Room
{
    int sector = 0;
    bool real = false;
};

static Room ParseRoom(std::string_view room)
{
    Room result;
    auto parts = Split(room, '-');
    auto const sectorChecksum = Split(parts.back(), '[');
    result.sector = svtoi(sectorChecksum[0]);
    auto const roomChecksum = pop_back(sectorChecksum[1]);

    parts.pop_back();
    std::unordered_map<char, int> letterMap{};

    for (auto letters : parts)
    {
        for (char c : letters)
        {
            ++letterMap[c];
        }
    }

    std::vector<std::pair<char, int>> sortedLetters(letterMap.size());

    auto first = begin(sortedLetters);
    auto mid = first + 5;

    std::transform(begin(letterMap), end(letterMap), first,
        [](auto const &p)
        {
            return p;
        });

    std::sort(first, end(sortedLetters),
        [](auto const &a, auto const &b)
        {
            if (b.second == a.second)
                return a.first < b.first;

            return b.second < a.second;
        });

    std::string calculatedChecksum(5, '\0');
    std::transform(first, mid, begin(calculatedChecksum),
        [](auto const &p)
        {
            return p.first;
        });

    result.real = calculatedChecksum == roomChecksum;
    return result;
}

static std::vector<Room> ParseRooms(std::string_view rooms)
{
    auto const lines = Split(rooms, '\n');
    std::vector<Room> result(lines.size());
    std::transform(begin(lines), end(lines), begin(result), ParseRoom);
    return result;
}

static auto SumSectorIdOfRealRooms(std::string_view rooms)
{
    int sum = 0;
    for (Room r : ParseRooms(rooms))
    {
        if (r.real)
        {
            sum += r.sector;
        }
    }

    return sum;
}

static auto Part1()
{
    return SumSectorIdOfRealRooms(input::rooms);
}

static auto Part2()
{
    return 0;
}

int main()
{
    // https://adventofcode.com/2016/day/4
    fmt::print("Day 4, 2016: Security Through Obscurity\n");

    Assert(ParseRoom(example::room1).real);
    Assert(ParseRoom(example::room2).real);
    Assert(ParseRoom(example::room3).real);
    Assert(not ParseRoom(example::room4).real);

    auto const part1 = Part1();
    fmt::print("  Part 1: {}\n", part1);
    Assert(137896 == part1);

    auto const part2 = Part2();
    fmt::print("  Part 2: {}\n", part2);
    // Assert( == part2);
}
