#include "day2.hpp"
#include "../common/assert.hpp"
#include "../common/string.hpp"
#include <charconv>
#include <fmt/format.h>
#include <sstream>

struct Box
{
    int width = 0;
    int height = 0;
    int depth = 0;

    explicit Box(std::string_view boxText)
    {
        std::istringstream ss(std::string{boxText});
        std::string valueText;
        std::getline(ss, valueText, 'x');
        width = std::stoi(valueText);
        std::getline(ss, valueText, 'x');
        height = std::stoi(valueText);
        std::getline(ss, valueText, 'x');
        depth = std::stoi(valueText);
    }

    [[nodiscard]] constexpr int area() const
    {
        int area_fb = width * height;
        int area_tb = width * depth;
        int area_lr = height * depth;
        return 2 * area_fb + 2 * area_tb + 2 * area_lr + std::min(area_fb, std::min(area_tb, area_lr));
    }

    [[nodiscard]] constexpr int length() const
    {
        int perim_fb = (width + height) * 2;
        int perim_tb = (width + depth) * 2;
        int perim_lr = (height + depth) * 2;
        return std::min(perim_fb, std::min(perim_tb, perim_lr)) + width * height * depth;
    }
};

static std::pair<int, int> calculate()
{
    auto const lines = Split(GetInput(), '\n');
    int total_area = 0;
    int total_length = 0;

    for (auto const &line : lines)
    {
        Box b{line};
        total_area += b.area();
        total_length += b.length();
    }

    return {total_area, total_length};
}

int main()
{
    // https://adventofcode.com/2015/day/2
    fmt::print("Day 2, 2015: I Was Told There Would Be No Math\n");

    Assert(58 == Box{"2x3x4"}.area());
    Assert(43 == Box{"1x1x10"}.area());
    Assert(34 == Box{"2x3x4"}.length());
    Assert(14 == Box{"1x1x10"}.length());

    auto const [area, length] = calculate();

    fmt::print("  Part1: {}\n", area);
    fmt::print("  Part2: {}\n", length);

    Assert(1586300 == area);
    Assert(3737498 == length);
}
