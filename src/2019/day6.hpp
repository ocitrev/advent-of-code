#pragma once
#include "input-bytes.hpp"
#include <string_view>

namespace example
{
    static constexpr std::string_view data{R"(COM)B
B)C
C)D
D)E
E)F
B)G
G)H
D)I
E)J
J)K
K)L
K)YOU
I)SAN)"};
}
