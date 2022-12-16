#pragma once
#include "string.hpp"
#include <vector>

template <typename IntT = int, char separator = '\n'>
inline std::vector<IntT> ParseInputNumbers()
{
    std::vector<IntT> numbers;

    for (auto number : Split(GetInput(), separator))
    {
        numbers.push_back(svtoi<IntT>(number));
    }

    return numbers;
}
