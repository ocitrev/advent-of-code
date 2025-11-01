#pragma once
#include "input-bytes.hpp"

#include <array>
#include <string_view>

namespace example
{
    using namespace std::string_view_literals;
    [[maybe_unused]] static constexpr std::array instructions{
        "mask = 000000000000000000000000000000X1001X"sv,
        "mem[42] = 100"sv,
        "mask = 00000000000000000000000000000000X0XX"sv,
        "mem[26] = 1"sv,
    };
}
