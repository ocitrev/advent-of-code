#pragma once
#include "input-bytes.hpp"
#include <array>
#include <string_view>

namespace example
{
    using namespace std::literals;
    static constexpr auto instructions = R"(ULL
RRDDD
LURDL
UUUUD)"sv;
}

namespace details
{
    using namespace std::literals;
    static constexpr std::string_view keypad1 = R"(1 2 3
4 5 6
7 8 9)"sv;

    static constexpr std::string_view keypad2 = R"(    1
  2 3 4
5 6 7 8 9
  A B C
    D)"sv;
}

using details::keypad1;
using details::keypad2;
