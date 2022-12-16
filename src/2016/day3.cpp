#include "day3.hpp"
#include "../common.hpp"

using Triangle = std::array<int, 3>;

static Triangle ParseTriangle(std::string_view triangleText)
{
    auto numbers = Split(triangleText, ' ');
    static_cast<void>(std::remove(begin(numbers), end(numbers), std::string_view{}));
    Triangle triangle;
    triangle[0] = svtoi(numbers[0]);
    triangle[1] = svtoi(numbers[1]);
    triangle[2] = svtoi(numbers[2]);
    return triangle;
}

static bool IsTriangleValid(Triangle triangle)
{
    std::sort(begin(triangle), end(triangle));
    return triangle[0] + triangle[1] > triangle[2];
}

static bool IsTriangleTextValid(std::string_view triangleText)
{
    return IsTriangleValid(ParseTriangle(triangleText));
}

static int Part1()
{
    return TransformReduceLines(GetInput(), 0, std::plus{}, &IsTriangleTextValid);
}

static std::vector<Triangle> ParseTrianglesVerticaly(std::string_view text)
{
    auto const rows = Split(text, '\n');
    std::vector<Triangle> triangles(rows.size());
    int line = 0;
    size_t col = 0;

    for (auto const &row : rows)
    {
        Triangle const triangle = ParseTriangle(row);
        size_t const triangleIndex = static_cast<size_t>(line / 3) * 3;
        size_t const colIndex = static_cast<size_t>(col % 3);
        triangles[triangleIndex + 0][colIndex] = triangle[0];
        triangles[triangleIndex + 1][colIndex] = triangle[1];
        triangles[triangleIndex + 2][colIndex] = triangle[2];
        ++col;
        ++line;
    }

    return triangles;
}

static int CountVerticalTriangles(std::vector<Triangle> const &triangles)
{
    return std::transform_reduce(begin(triangles), end(triangles), 0, std::plus{}, IsTriangleValid);
}

static int Part2()
{
    return CountVerticalTriangles(ParseTrianglesVerticaly(GetInput()));
}

int main()
{
    // https://adventofcode.com/2016/day/3
    fmt::print("Day 3, 2016: Squares With Three Sides\n");

    Assert(false == IsTriangleTextValid(example::triangle));
    Assert(6 == CountVerticalTriangles(ParseTrianglesVerticaly(example::triangles2)));

    auto const part1 = Part1();
    fmt::print("  Part 1: {}\n", part1);
    Assert(982 == part1);

    auto const part2 = Part2();
    fmt::print("  Part 2: {}\n", part2);
    Assert(1826 == part2);
}
