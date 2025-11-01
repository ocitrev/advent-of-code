#pragma once
#include "input-bytes.hpp"

#include <string_view>

namespace example
{
    constexpr std::string_view grammar1{R"(H => HO
H => OH
O => HH)"};

    constexpr std::string_view grammar2{R"(e => H
e => O
H => HO
H => OH
O => HH)"};

    constexpr std::string_view molecule1{"HOH"};
    constexpr std::string_view molecule2{"HOHOHO"};
}
