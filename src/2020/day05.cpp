#include "day05.hpp"
#include "../common/assert.hpp"
#include <algorithm>
#include <fmt/format.h>
#include <set>

struct Seat
{
    int row = 0;
    int col = 0;

    [[nodiscard]] int GetId() const
    {
        return row * 8 + col;
    }

    bool operator<(Seat const &s) const
    {
        return std::make_pair(row, col) < std::make_pair(s.row, s.col);
    }

    bool operator==(Seat const &s) const
    {
        return row == s.row && col == s.col;
    }
};

Seat GetSeat(std::string_view boardingPass)
{
    int low = 0;
    int high = 127;

    for (char c : boardingPass.substr(0, 7))
    {
        int const middle = ((high - low + 1) / 2);
        if (c == 'F')
            high -= middle;
        else
            low += middle;
    }

    int left = 0;
    int right = 7;

    for (char c : boardingPass.substr(7, 3))
    {
        int const middle = ((right - left + 1) / 2);
        if (c == 'L')
            right -= middle;
        else
            left += middle;
    }

    return Seat{low, left};
}

int Part1()
{
    std::array<int, input::passes.size()> ids{};
    std::transform(begin(input::passes), end(input::passes), begin(ids),
                   [](auto pass)
                   {
                       return GetSeat(pass).GetId();
                   });

    return *std::max_element(begin(ids), end(ids));
}

int Part2()
{
    std::set<Seat> seats;
    for (auto &&pass : input::passes)
    {
        seats.insert(GetSeat(pass));
    }

    bool found = false;

    for (int r = 1; r < 127; ++r)
    {
        for (int c = 0; c < 8; ++c)
        {
            Seat const s{r, c};

            if (seats.find(s) == end(seats))
            {
                // fmt::print("Seat({}, {}) is missing.\n", s.row, s.col);
                if (found)
                    return s.GetId();
            }
            else
            {
                found = true;
            }
        }
    }

    return 0;
}

int main()
{
    // https://adventofcode.com/2020/day/5
    fmt::print("Day 5, 2020: Binary Boarding\n");

    Assert(357 == GetSeat("FBFBBFFRLR").GetId());
    Assert(567 == GetSeat("BFFFBBFRRR").GetId());
    Assert(119 == GetSeat("FFFBBBFRRR").GetId());
    Assert(820 == GetSeat("BBFFBBFRLL").GetId());

    auto const part1 = Part1();
    fmt::print("  Part 1: {}\n", part1);
    Assert(883 == part1);

    auto const part2 = Part2();
    fmt::print("  Part 2: {}\n", part2);
}
