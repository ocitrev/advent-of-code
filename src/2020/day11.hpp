#pragma once
#include "input-bytes.hpp"

#include <string_view>

namespace example
{
    static constexpr std::string_view seats{R"(L.LL.LL.LL
LLLLLLL.LL
L.L.L..L..
LLLL.LL.LL
L.LL.LL.LL
L.LLLLL.LL
..L.L.....
LLLLLLLLLL
L.LLLLLL.L
L.LLLLL.LL)"};

    static constexpr std::string_view visibleSteats1{R"(.......#.
...#.....
.#.......
.........
..#L....#
....#....
.........
#........
...#.....)"};

    static constexpr std::string_view visibleSteats2{R"(.............
.L.L.#.#.#.#.
.............)"};
    static constexpr std::string_view visibleSteats3{R"(.##.##.
#.#.#.#
##...##
...L...
##...##
#.#.#.#
.##.##.)"};

}
