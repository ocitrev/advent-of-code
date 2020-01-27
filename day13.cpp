#include "day13.hpp"
#include "common/intcode.hpp"
#include "common/main.hpp"
#include "common/point.hpp"
#include <array>
#include <cassert>
#include <iostream>
#include <map>

enum class Tile
{
    Empty,  // No game object appears in this tile.
    Wall,   // Walls are indestructible barriers.
    Block,  // Blocks can be broken by the ball.
    Paddle, // The paddle is indestructible.
    Ball,   // The ball moves diagonally and bounces off objects.
};

void Part1()
{
    Point pos;
    std::map<Point, Int> grid;

    auto setX = [&](Int value) {
        pos.x = static_cast<int>(value);
    };

    auto setY = [&](Int value) {
        pos.y = static_cast<int>(value);
    };

    auto setTitle = [&](Int value) {
        grid[pos] = value;
    };

    std::array<std::function<void(Int)>, 3> states = {setX, setY, setTitle};

    Intcode::Run(input::code, nullptr, [&](Int value) {
        states.front()(value);
        std::rotate(begin(states), begin(states) + 1, end(states));
    });

    auto result = std::count_if(begin(grid), end(grid), [](auto const &elem) {
        return elem.second == static_cast<Int>(Tile::Block);
    });

    std::cout << "Part1: " << result << '\n';
    assert(expected::part1 == result);
}

void Main()
{
    std::cout << "Day 13: Care Package\n";

    Part1();
}
