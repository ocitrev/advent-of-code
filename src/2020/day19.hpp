#pragma once
#include "input-bytes.hpp"

#include <string_view>

namespace example
{
    constexpr std::string_view rules1 =
        R"(0: 1 2
1: "a"
2: 1 3 | 3 1
3: "b")";

    constexpr std::string_view rules2 = R"(0: 4 1 5
1: 2 3 | 3 2
2: 4 4 | 5 5
3: 4 5 | 5 4
4: "a"
5: "b")";

    constexpr std::string_view values2 = R"(ababbb
bababa
abbbab
aaabbb
aaaabbb)";
}
