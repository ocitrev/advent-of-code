#pragma once
#include "input-bytes.hpp"

#include <string_view>

namespace example
{
    using namespace std::string_view_literals;
    [[maybe_unused]] static constexpr std::string_view instructions{R"(nop +0
acc +1
jmp +4
acc +3
jmp -3
acc -99
acc +1
jmp -4
acc +6)"};
}
