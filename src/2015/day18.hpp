#pragma once
#include "input-bytes.hpp"

#include <string_view>

namespace example1
{
    constexpr std::string_view state0{R"(.#.#.#
...##.
#....#
..#...
#.#..#
####..)"};
    constexpr std::string_view state1{R"(..##..
..##.#
...##.
......
#.....
#.##..)"};
    constexpr std::string_view state2{R"(..###.
......
..###.
......
.#....
.#....)"};
    constexpr std::string_view state3{R"(...#..
......
...#..
..##..
......
......)"};
    constexpr std::string_view state4{R"(......
......
..##..
..##..
......
......)"};
}

namespace example2
{
    constexpr std::string_view state0{R"(##.#.#
...##.
#....#
..#...
#.#..#
####.#)"};
    constexpr std::string_view state1{R"(#.##.#
####.#
...##.
......
#...#.
#.####)"};
    constexpr std::string_view state2{R"(#..#.#
#....#
.#.##.
...##.
.#..##
##.###)"};
    constexpr std::string_view state3{R"(#...##
####.#
..##.#
......
##....
####.#)"};
    constexpr std::string_view state4{R"(#.####
#....#
...#..
.##...
#.....
#.#..#)"};
    constexpr std::string_view state5{R"(##.###
.##..#
.##...
.##...
#.#...
##...#)"};
}
