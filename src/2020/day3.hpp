#pragma once
#include <string_view>

namespace example
{
    [[maybe_unused]] static constexpr std::string_view map{R"(..##.......
#...#...#..
.#....#..#.
..#.#...#.#
.#...##..#.
..#.##.....
.#.#.#....#
.#........#
#.##...#...
#...##....#
.#..#...#.#)"};
}

namespace input
{

    static constexpr std::string_view map{R"(...............#...#..#...#....
...#....##.....##...######..#..
....#.....#.##..#...#..#.......
...#..........#.#....#......#..
....#.#...#.#.......#......##.#
....#.#..........#.....#.##....
##...#.#.##......#......#.#.#..
#.#.#........#....#..#.#.......
..#...##..#..#.......#....###..
.#.#..........#...#........#..#
.#..#......#....#.#...#...#.#.#
..#.........#..##.....#.#.##.#.
.#......#...#....#.....#......#
........#..##..##.........#..#.
.....#....###..#....##........#
.###...#..##..#.##......##...##
.#...#...#..#.#..#...##.....#..
.......#....#....#.#...#.......
.##.......#.##...#.#...#..#....
#.#...#......#....#.......#....
..###...............####...#.#.
.##.#....#......#..#...#.#..#.#
.............#.#.#......##.....
#....#.#.#........#....##...#..
...##....##...##..#...#........
..##......#...#......#...###...
...#...##......##.#.#..#.......
#......#..#...#.#..#......#..##
.#..#..#........##....##.......
.#...........###.###.##...#....
............#.#...........#.#..
#...#........#...#...#..#.#.#.#
...#.......#.#.#..#.#..........
......#..#..#....##..##..##....
........##...#......#..#.#.....
..#.#.......#......#...........
#.#.....#......##..........#.#.
#.....###.#...#...#..#....#...#
.##.#...#............##.....#..
###....#.#.....#.......##......
##.......##.....#..#...#..##.##
....#.##............###...#..##
.###..#...##.#..#..##..#.......
.##.##..####.....#.........#...
....####..#...#....#.#...#.....
..##....#..#......#...........#
..........#......#..##.......#.
.................#.#.#........#
#.......##.#...##.......##.##..
.#................#.#.....##..#
......#..#............##.#.....
...##............#.....#.....#.
##.###.#.......#....#.........#
......#.....#.#.#.#......#.....
......#.##......#......##......
..#....#...#..#.....#..#....#.#
.#.##.##.....#.......#.......#.
...#..#.#......##....##..#.....
.#.....#......##...#..#.#....#.
..#......#....#..#..###.#.#....
.....#........#.#...#..#.#.....
....#.#.......#...##.....####..
#..#..##...#...........#...#..#
.#..#...#.....#.....#.#.....#.#
..##..###.....#...#.#.#.......#
#..##.##......###..#......###..
#..#...#.......#....#.#...#.#.#
...........###..#...#..##....#.
.....#...........###.......#...
##......#.......#......##.#..#.
#.................#........#...
#.#.............#......#...#..#
......#.#....#....#....#....#.#
..#...#....#..#....#....#..#...
#....#......#..#...#..#....#.#.
..#.....#..#...#...#.......#...
.#........###..##.#..#.........
.....##.#.....#..###..#........
...#...#.###....######......#..
.###.#..#.....#.....#..#...#...
##..#.#......#.........#...#..#
###...##..###.......#..##.#.#.#
.#...................#..#...##.
.#...#...###...#.......#......#
....#.....##....#...##.#...#.##
..#....#......#...#..###.......
.........#..........##........#
...#.........##..#....#..###...
......#.##....#.#........#.#.##
....#..#...#.............#....#
#..#.....#.#.....#....#........
....#..#...####.#.###..#.......
.....#....#....#..#..#.#.....#.
#..##....#.....#.#.............
.##...#..#.......#...##.###....
##......#...##....#......##....
#......#.#......#.#..#......#..
.#...#......#............###..#
.#..#.#.....#.#.....#..#....#..
.#............#...#..#..#...##.
...#...#....#.#.#....#....#....
........#......###.#....##.##.#
......#.#..................##..
..#..#........#..##..........##
.#...#.#....#####.#.#..#.......
.....#..#.#..#.....#.#........#
#.#..#..#.#..........#..#..#...
.##........#.#.......#........#
.....#....#..##...#......##....
....#.##.##...##.#.........#.#.
...#...#..#.#.......#.......#..
.................##..#.........
..##.##....#...#.##.......#..#.
....#...........#.....###....##
.#..................#..........
....#.##....#......##.#..#.##.#
#......#..#.#....##...####...#.
#.....#.#.##...........#.##...#
.......#...##..#.........##....
.#...#..........##......#..#.#.
#...#.#......#.....#........#..
........#.....#.......##......#
.#..#...........#...#..#..#....
......#.##......##.#......#..##
......#....#..#................
##.#.......#......#..#....##.##
..#...###..#.......#...#.#....#
.....#...#.........#.....#..#.#
..#.....#.........#..#...#.#...
.#.........##..#.#...#.....##..
..........#.#.#...#....#....#..
....#.###.#..#..#..#.#........#
..#...##...##.#.#.....#.#..##..
.#..#......#.####..#.......#..#
##.......#.....#.#.#..#...##..#
.##......##...##.........#..#..
..#.##..#......#......#..#..#..
..#..#.##..#........#....#.#...
##.####...##.#..##.........#..#
.#.......#.#..#.....#.##.......
........#.........#...........#
..#...#.####.....##.##.#....#.#
.....#..##.#..###.##.#.#...#...
#..##..##....#...#....#...#....
.###.#....#..#.......#......###
.#....##.....#.#........#...#.#
#.#.#..#..#...#....#..#.....#..
....##...#.##..#............#..
##......###...#...#...#....#...
#.#...#.#..#..##.##..##..#....#
.#...#.#....#..##.#..##..#.....
.............#..#..#.#.....#...
#........#..........#....###...
..#..#......#...#........#.....
.#..#.............#.........#..
#.....#....##..#..#.#.#..#.....
...#......#.........#.#....##.#
..#.......##....###..#.........
.#.......#......#..............
.#...##.....##...###.#....#.#..
......#....#.........#.......#.
##.......#..##....###.#.....##.
.....#......####.....#......#..
....#....#..###....#.....##...#
...#...#.#........#.....#..#.##
#..#....#.#...###...##.....##..
#.....##...#.#............#....
##....#.......#.#.#....#.#.###.
#........#...#...####.#....#.#.
.##.#......#........#..#.....#.
...##.#.......#...#.#.##..#...#
......#.........##..#....#.....
.#......#...........#......#...
......#.#.#.#..#.#....#....#..#
##.................##...#.#....
........#.........#..#..#...###
.#........#........#....#..#...
....###.##.##......#.#...#....#
#......#.#..............#......
.......#..#....#..##.........#.
#............#....#............
#...#.#.........##..###...##...
.#....#.#.#.....#..#..##.......
.............##.#.#.......#..#.
#...#..##.#..###.....##..#.....
...#.#.....#...#......##.#..#..
#..........#.##.#...#...#.#...#
...#.#...#.........#.#..#.#..#.
#....#.................#.......
..#..#.#.#..#.#..##...#.#......
..#....#.#.#...#.....#...#.....
#...#.###......#.......#..#..#.
#.#.##.##..............#.#.#..#
#..........#.#.........#.###...
...........#.......#.#..#......
....#.#..#....#....#....##.....
#..#...##########.........#.#..
..#.............##........#.#..
#.#.##......#...#.#....##..##..
..##..#.#.#....#......#.#..#.#.
.#.#...#................#......
#...#...#.....##.#...#.......#.
.....##.......#...#......#.##..
....#.##..........#.#.##....##.
...........##........#.#.#.#...
..#...........###.#....#..#....
..#.#...#...#.#........#.....#.
.##......##.....#........#.....
....#.......#....#...#.##...#..
.#.....##.....#...##...........
#....#.##.##...#...###.#####...
..#.#......#.#.......#....#..#.
..........#...#...........#....
.........#..#...#...#......#.##
.#...#.#...#.###.##..#........#
#....#.....#.......##....#.....
#.....#..#.....#...##.......#..
#.#.#.#.............#.....#...#
...#.....#.....#...............
..##.###.#.#........#.........#
...##..#.##..#....#.#...#.#....
...##...#...##.#.........#.#..#
.###......#....#.........#.#...
###.#.#...#.......#...#.....#.#
...#.#.......#.....####........
......#..#.....###.#....#..#...
..####...#...#..#......#...#...
#..............##....#......##.
..##..###...##..#.#.........#..
#..#.#...#.......#.........##..
..##....#......#...#..##.......
..#.#..#..###.....#.....#...###
.#..#.##.....##.#.#.#........#.
..#.#.........#................
..#...........#.#...##.#...#..#
.....#........#..#.....#....#..
#.#....#...........##.....#..##
##.......#.....#.....#.#......#
.##............####.#........##
....##.#.....#......#....#...#.
.#.#...##.#..##..#..........#..
..........................#....
##..##..#..#.#....#.....#......
...#.#........#.#.##.#.....#..#
#..#....#...#..#...#........#.#
#.......#####...#..#..#.#......
#.##....#......#......#..###...
..#.......#...........#.....##.
#...#....#..#......##...#......
...##.#..##........#..###......
##.#...........#.............##
#.....#..#.....#.....#.........
.#..........#..#......###......
...#...#..##.......#...#...#.#.
..####......#.....#..#.#......#
....#..#.....#.#...............
.#.......#....#.....#..##....#.
.....#.........#..........##...
#...........#..#.....#........#
............#..##...#...#.#....
##.............####...#.....#..
.....#......#....##.#.....##...
...........#.....#.#..#.#......
.#.#......#....#.............##
##...#.#.......##........#.....
#..#.....#.#.......####...#..#.
....#.#...#....#.###..#..#.#...
.....#.#..#......#.##.#####.#..
.....#....##..........#......#.
#.......#........##.......##...
#...#..#..##.#....#...#...#....
...#..##..#...#..........#.....
#..#....###.....#......##......
...###......#.....#....#.......
#.#...#.#..###.####.......#.#.#
...#......#.#..##..#.....#.....
#.#............#.....##.#......
#..#......##...##..#...#.#..###
#.....#...#..#..#....#..###....
####..###....#..##....#..#.....
..##.#.....#.......##....#.#.#.
##..#.#.......#.#...##.#..#.#..
..#.#.#.##.#.#.#...........#...
...#.##.....#....#..#.#..#.....
...#..#.........#..........#..#
#...#..#.....#.#.#.............
##.#....##.##...#...#..#..#..#.
....####..##..##.#..#...##.....
##.....##.#.#...#.#.......###..
#..#.#....#......#.......##...#
#.#...............#..#..#......
.....##...##..#........#......#
.#..#............##......#....#
......#.#..#..##.#......#.....#
..#.#.............#...#......##
....#.#..#..#...##...#..##.....
#.#.............#...#.#..#....#
#..#..#.##....#....#...#.......
....#..#..........#.....##...#.
..#####.......#...#..........#.
....#......##.......#..##.#.#.#
#...#.#.....#....#....##.......
..##.#.#..#.#...#.....##.....#.
#.#..#....#............#..#.#..
...#.##..##..#.#...###......#.#
......##.......#....#......###.
....#..##.......#..#.#....#..#.
#............#..........##..#.#
..#.....#..#.#..##..#....##.#..
.....#.....#....##.#.#......#.#
...####.......###..#...#.#....#
.#.##.....##.....##..#..#......
...#..###..##..................
##..##.....#.#..#..#.#........#
.#.#........##.........#....#..
........#......###....#...#....
......#...........#...........#
.#...###...#.........#.#...#..#
.....#..........#......##....#.
.#.#...#........##.......#.#...
.....##.....##....#...#...#..#.
..#.....................##...##
#..#.#......##...##..#......#..)"};
}