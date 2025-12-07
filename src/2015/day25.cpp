#include "day25.hpp"

#include "../common/assert.hpp"
#include "../common/string.hpp"

#include <print>

[[maybe_unused]] static int64_t GetNextSimple(int64_t n, bool first)
{
    if (first)
    {
        return 1;
    }

    return ++n;
}

static int64_t GetNext(int64_t n, bool first)
{
    if (first)
    {
        return int64_t{20'151'125};
    }

    return (n * int64_t{252'533}) % int64_t{33'554'393};
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
            {
                return n;
            }

            --y;
            ++x;
            n = getNext(n, false);
        }

        ++yStart;
    }
}

static std::pair<int, int> ParseInput()
{
    auto const text = GetInput();
    int const row = svtoi(text.substr(text.find("row ") + 4));
    int const column = svtoi(text.substr(text.find("column ") + 7));
    return std::make_pair(column, row);
}

int main()
{
    // https://adventofcode.com/2015/day/25
    std::print("Day 25, 2015: Let It Snow\n");

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
    Assert(20'151'125 == GetValue(1, 1, &GetNext));
    Assert(18'749'137 == GetValue(2, 1, &GetNext));
    Assert(17'289'845 == GetValue(3, 1, &GetNext));
    Assert(30'943'339 == GetValue(4, 1, &GetNext));
    Assert(10'071'777 == GetValue(5, 1, &GetNext));
    Assert(33'511'524 == GetValue(6, 1, &GetNext));
    Assert(31'916'031 == GetValue(1, 2, &GetNext));
    Assert(21'629'792 == GetValue(2, 2, &GetNext));
    Assert(16'929'656 == GetValue(3, 2, &GetNext));
    Assert(7'726'640 == GetValue(4, 2, &GetNext));
    Assert(15'514'188 == GetValue(5, 2, &GetNext));
    Assert(16'080'970 == GetValue(1, 3, &GetNext));
    Assert(8'057'251 == GetValue(2, 3, &GetNext));
    Assert(1'601'130 == GetValue(3, 3, &GetNext));
    Assert(7'981'243 == GetValue(4, 3, &GetNext));
    Assert(24'592'653 == GetValue(1, 4, &GetNext));
    Assert(32'451'966 == GetValue(2, 4, &GetNext));
    Assert(21'345'942 == GetValue(3, 4, &GetNext));
    Assert(77'061 == GetValue(1, 5, &GetNext));
    Assert(17'552'253 == GetValue(2, 5, &GetNext));
    Assert(33'071'741 == GetValue(1, 6, &GetNext));

    auto const [column, row] = ParseInput();
    auto const part1 = GetValue(column, row, &GetNext);
    std::print("  Part 1: {}\n", part1);
    Assert(9'132'360 == part1);
}
