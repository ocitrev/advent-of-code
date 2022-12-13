#include "day4.hpp"
#include "../common.hpp"

using namespace std::literals;

static std::string_view pop_back(std::string_view text)
{
    text.remove_suffix(1);
    return text;
}

struct Decryptor
{
    int key;

    Decryptor(int key_)
        : key{key_}
    {
    }

    char operator()(char c) const
    {
        return ((c - 'a') + key) % 26 + 'a';
    }

    std::string operator()(std::string_view text) const
    {
        std::string result(text.size(), '\0');
        std::transform(begin(text), end(text), begin(result), *this);
        return result;
    }
};

struct Room
{
    int sector = 0;
    bool real = false;
    std::vector<std::string> words;

    std::string Decrypt() const
    {
        std::vector<std::string> result(words.size());
        std::transform(begin(words), end(words), begin(result), Decryptor{sector});
        return Join(begin(result), end(result), " "sv);
    }
};

static Room ParseRoom(std::string_view room)
{
    Room result;
    auto parts = Split(room, '-');
    auto const sectorChecksum = Split(parts.back(), '[');
    result.sector = svtoi(sectorChecksum[0]);
    auto const roomChecksum = sectorChecksum.size() > 1 ? pop_back(sectorChecksum[1]) : std::string_view{};

    parts.pop_back();
    std::unordered_map<char, int> letterMap{};

    for (auto letters : parts)
    {
        result.words.emplace_back(letters);

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

static int FindSectorIdForRoom(std::string_view rooms, std::string_view compare)
{
    for (Room r : ParseRooms(rooms))
    {
        auto const roomName = r.Decrypt();
#if 0
        fmt::print("{}-{}\n", roomName, r.sector);
#endif

        if (roomName.find(compare) != std::string_view::npos)
        {
            return r.sector;
        }
    }

    return 0;
}

static auto Part1()
{
    return SumSectorIdOfRealRooms(input::rooms);
}

static auto Part2()
{
    return FindSectorIdForRoom(input::rooms, "object"sv);
}

int main()
{
    // https://adventofcode.com/2016/day/4
    fmt::print("Day 4, 2016: Security Through Obscurity\n");

    Assert(ParseRoom(example::room1).real);
    Assert(ParseRoom(example::room2).real);
    Assert(ParseRoom(example::room3).real);
    Assert(not ParseRoom(example::room4).real);
    Assert("very encrypted name" == ParseRoom(example::room5).Decrypt());

    auto const part1 = Part1();
    fmt::print("  Part 1: {}\n", part1);
    Assert(137896 == part1);

    auto const part2 = Part2();
    fmt::print("  Part 2: {}\n", part2);
    Assert(501 == part2);
}
