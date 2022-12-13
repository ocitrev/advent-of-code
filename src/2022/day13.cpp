#include "day13.hpp"
#include "../common.hpp"
#include <fstream>
#include <variant>

static std::string ReadInput()
{
    std::ifstream input("../inputs/2022/day13.txt", std::ios_base::ate);
    if (input.bad())
        throw std::invalid_argument("Failed to open file");

    std::string result;
    result.reserve(static_cast<size_t>(input.tellg()));
    input.seekg(0, std::ios::beg);
    result.assign(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());
    return result;
}

struct Packet
{
    std::vector<std::variant<Packet, int>> items;
    bool marker = false;
};

Packet ParsePacket(std::string_view text, bool marker = false)
{
    text.remove_prefix(1);
    text.remove_suffix(1);

    std::vector<Packet> stack(1);
    stack.back().marker = marker;
    auto const last = end(text);
    auto start = begin(text);
    auto stop = start;

    for (auto iter = start; iter != last; ++iter)
    {
        if (*iter == '[')
        {
            start = iter + 1;
            stack.emplace_back();
        }
        else if (*iter == ']')
        {
            stop = iter;

            if (start != stop)
            {
                stack.back().items.push_back(svtoi({start, stop}));
            }

            start = iter + 1;
            Packet p = std::move(stack.back());
            stack.pop_back();
            stack.back().items.push_back(std::move(p));
        }
        else if (*iter == ',')
        {
            stop = iter;

            if (start != stop)
            {
                stack.back().items.push_back(svtoi({start, stop}));
            }

            start = iter + 1;
            stop = start;
        }
    }

    if (start != last)
    {
        stack.back().items.push_back(svtoi({start, last}));
    }

    return stack.back();
}

static int Compare(Packet const &left, Packet const &right)
{
    auto leftLast = end(left.items);
    auto rightLast = end(right.items);

    auto leftIt = begin(left.items);
    auto rightIt = begin(right.items);

    while (true)
    {
        if (leftIt == leftLast)
        {
            if (rightIt == rightLast)
            {
                return 0;
            }

            return -1;
        }

        if (rightIt == rightLast)
        {
            return 1;
        }

        if (leftIt->index() == rightIt->index())
        {
            if (leftIt->index() == 1)
            {
                if (std::get<1>(*leftIt) < std::get<1>(*rightIt))
                {
                    return -1;
                }
                else if (std::get<1>(*leftIt) > std::get<1>(*rightIt))
                {
                    return 1;
                }
            }
            else
            {
                if (auto const cmp = Compare(std::get<0>(*leftIt), std::get<0>(*rightIt)); cmp != 0)
                {
                    return cmp;
                }
            }
        }
        else if (leftIt->index() == 1)
        {
            Packet leftPacket;
            leftPacket.items.push_back(std::get<1>(*leftIt));

            if (auto const cmp = Compare(leftPacket, std::get<0>(*rightIt)); cmp != 0)
            {
                return cmp;
            }
        }
        else
        {
            Packet rightPacket;
            rightPacket.items.push_back(std::get<1>(*rightIt));

            if (auto const cmp = Compare(std::get<0>(*leftIt), rightPacket); cmp != 0)
            {
                return cmp;
            }
        }

        ++leftIt;
        ++rightIt;
    }
}

static int Compare(std::string_view pair)
{
    auto lines = Split(pair, '\n');
    auto left = ParsePacket(lines[0]);
    auto right = ParsePacket(lines[1]);
    return Compare(left, right);
}

static int SumOrderedPairs(std::string_view input)
{
    int sum = 0;
    int pairIndex = 0;
    for (auto chunk : Split(input, "\n\n"))
    {
        ++pairIndex;

        if (int const cmp = Compare(chunk); cmp <= 0)
        {
            sum += pairIndex;
        }
    }

    return sum;
}

static std::vector<Packet> ParsePackets(std::string_view input)
{
    std::vector<Packet> packets;
    packets.push_back(ParsePacket("[[2]]", 2));
    packets.push_back(ParsePacket("[[6]]", 6));

    for (auto line : Split(input, '\n'))
    {
        if (line.empty())
            continue;

        packets.push_back(ParsePacket(line));
    }

    return packets;
}

static void SortPackets(std::vector<Packet> &packets)
{
    std::sort(begin(packets), end(packets),
        [](Packet const &left, Packet const right)
        {
            return Compare(left, right) < 0;
        });
}

static int FindDecoderKey(std::string_view input)
{
    auto packets = ParsePackets(input);
    SortPackets(packets);

    int key = 1;
    int index = 0;

    for (auto const &p : packets)
    {
        ++index;

        if (p.marker)
        {
            key *= index;
        }
    }

    return key;
}

static auto Part1()
{
    return SumOrderedPairs(ReadInput());
}

static auto Part2()
{
    return FindDecoderKey(ReadInput());
}

int main()
{
    // https://adventofcode.com/2022/day/13
    fmt::print("Day 13, 2022: Distress Signal\n");

    Assert(13 == SumOrderedPairs(example::signal));
    Assert(140 == FindDecoderKey(example::signal));

    auto const part1 = Part1();
    fmt::print("  Part 1: {}\n", part1);
    Assert(6484 == part1);

    auto const part2 = Part2();
    fmt::print("  Part 2: {}\n", part2);
    Assert(19305 == part2);
}
