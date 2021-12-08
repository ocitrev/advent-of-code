#pragma once

inline constexpr int Polygonal(int number, int side)
{
    return (((number * number) * (side - 2)) - number * (side - 4)) / 2;
}

inline constexpr int Triangular(int number)
{
    return Polygonal(number, 3);
}
