#include "day25.hpp"
#include "../common/assert.hpp"
#include <fmt/format.h>

[[maybe_unused]] static int64_t GetNextSimple(int64_t n, bool first)
{
    if (first)
        return 1;

    return ++n;
}

static int64_t GetNext(int64_t n, bool first)
{
    if (first)
        return int64_t{20151125};

    return (n * int64_t{252533}) % int64_t{33554393};
}

template <typename GetNextT>
int64_t GetValue(int c, int r, GetNextT &&getNext)
{
    int yStart = 1;
    int64_t n = getNext(0, true);

    while (true)
    {
        int y = yStart;
        int x = 1;

        while (y > 0)
        {
            if (x == c && y == r)
                return n;

            --y;
            ++x;
            n = getNext(n, false);
        }

        ++yStart;
    }
}

int main()
{
    // https://adventofcode.com/2015/day/25
    fmt::print("Day 25: Let It Snow\n");

    Assert(1 == GetValue(1, 1, &GetNextSimple));
    Assert(3 == GetValue(2, 1, &GetNextSimple));
    Assert(6 == GetValue(3, 1, &GetNextSimple));
    Assert(10 == GetValue(4, 1, &GetNextSimple));
    Assert(15 == GetValue(5, 1, &GetNextSimple));
    Assert(21 == GetValue(6, 1, &GetNextSimple));
    Assert(2 == GetValue(1, 2, &GetNextSimple));
    Assert(5 == GetValue(2, 2, &GetNextSimple));
    Assert(9 == GetValue(3, 2, &GetNextSimple));
    Assert(14 == GetValue(4, 2, &GetNextSimple));
    Assert(20 == GetValue(5, 2, &GetNextSimple));
    Assert(4 == GetValue(1, 3, &GetNextSimple));
    Assert(8 == GetValue(2, 3, &GetNextSimple));
    Assert(13 == GetValue(3, 3, &GetNextSimple));
    Assert(19 == GetValue(4, 3, &GetNextSimple));
    Assert(7 == GetValue(1, 4, &GetNextSimple));
    Assert(12 == GetValue(2, 4, &GetNextSimple));
    Assert(18 == GetValue(3, 4, &GetNextSimple));
    Assert(11 == GetValue(1, 5, &GetNextSimple));
    Assert(17 == GetValue(2, 5, &GetNextSimple));
    Assert(16 == GetValue(1, 6, &GetNextSimple));
    Assert(20151125 == GetValue(1, 1, &GetNext));
    Assert(18749137 == GetValue(2, 1, &GetNext));
    Assert(17289845 == GetValue(3, 1, &GetNext));
    Assert(30943339 == GetValue(4, 1, &GetNext));
    Assert(10071777 == GetValue(5, 1, &GetNext));
    Assert(33511524 == GetValue(6, 1, &GetNext));
    Assert(31916031 == GetValue(1, 2, &GetNext));
    Assert(21629792 == GetValue(2, 2, &GetNext));
    Assert(16929656 == GetValue(3, 2, &GetNext));
    Assert(7726640 == GetValue(4, 2, &GetNext));
    Assert(15514188 == GetValue(5, 2, &GetNext));
    Assert(16080970 == GetValue(1, 3, &GetNext));
    Assert(8057251 == GetValue(2, 3, &GetNext));
    Assert(1601130 == GetValue(3, 3, &GetNext));
    Assert(7981243 == GetValue(4, 3, &GetNext));
    Assert(24592653 == GetValue(1, 4, &GetNext));
    Assert(32451966 == GetValue(2, 4, &GetNext));
    Assert(21345942 == GetValue(3, 4, &GetNext));
    Assert(77061 == GetValue(1, 5, &GetNext));
    Assert(17552253 == GetValue(2, 5, &GetNext));
    Assert(33071741 == GetValue(1, 6, &GetNext));

    auto const part1 = GetValue(input::column, input::row, &GetNext);
    fmt::print("  Part 1: {}\n", part1);
    Assert(9132360 == part1);
}
