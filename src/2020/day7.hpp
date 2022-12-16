#pragma once
#include "input-bytes.hpp"
#include <array>
#include <string_view>

namespace example
{
    using namespace std::string_view_literals;
    [[maybe_unused]] static constexpr std::array rules1{
        "light red bags contain 1 bright white bag, 2 muted yellow bags."sv,
        "dark orange bags contain 3 bright white bags, 4 muted yellow bags."sv,
        "bright white bags contain 1 shiny gold bag."sv,
        "muted yellow bags contain 2 shiny gold bags, 9 faded blue bags."sv,
        "shiny gold bags contain 1 dark olive bag, 2 vibrant plum bags."sv,
        "dark olive bags contain 3 faded blue bags, 4 dotted black bags."sv,
        "vibrant plum bags contain 5 faded blue bags, 6 dotted black bags."sv,
        "faded blue bags contain no other bags."sv,
        "dotted black bags contain no other bags."sv,
    };

    [[maybe_unused]] static constexpr std::array rules2{
        "shiny gold bags contain 2 dark red bags."sv,
        "dark red bags contain 2 dark orange bags."sv,
        "dark orange bags contain 2 dark yellow bags."sv,
        "dark yellow bags contain 2 dark green bags."sv,
        "dark green bags contain 2 dark blue bags."sv,
        "dark blue bags contain 2 dark violet bags."sv,
        "dark violet bags contain no other bags."sv,
    };
}

namespace input
{
    using namespace std::string_view_literals;
    static constexpr auto mybag = "shiny gold"sv;
}
