#pragma once
#include "input-bytes.hpp"
#include <array>
#include <string_view>

namespace example
{
    using namespace std::string_view_literals;
    [[maybe_unused]] static constexpr std::array passwords{
        "1-3 a: abcde"sv,
        "1-3 b: cdefg"sv,
        "2-9 c: ccccccccc"sv,
    };
}
