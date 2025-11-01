#pragma once
#include "input-bytes.hpp"

#include <string_view>

namespace example
{
    using namespace std::literals;
    static constexpr auto room1 = "aaaaa-bbb-z-y-x-123[abxyz]"sv;
    static constexpr auto room2 = "a-b-c-d-e-f-g-h-987[abcde]"sv;
    static constexpr auto room3 = "not-a-real-room-404[oarel]"sv;
    static constexpr auto room4 = "totally-real-room-200[decoy]"sv;
    static constexpr auto room5 = "qzmt-zixmtkozy-ivhz-343";
}
