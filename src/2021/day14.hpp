#pragma once
#include "input-bytes.hpp"
#include <string_view>

namespace example
{
    constexpr std::string_view polymer{R"(NNCB

CH -> B
HH -> N
CB -> H
NH -> C
HB -> C
HC -> B
HN -> C
NN -> C
BH -> H
NC -> B
NB -> B
BN -> B
BB -> N
BC -> B
CC -> N
CN -> C)"};
}
