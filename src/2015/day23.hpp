#pragma once
#include <array>
#include <string_view>

namespace example
{
    static constexpr std::string_view instructions{R"(inc a
jio a, +2
tpl a
inc a)"};
}

namespace input
{
    static constexpr std::string_view instructions{
        R"(jio a, +16",
inc a
inc a
tpl a
tpl a
tpl a
inc a
inc a
tpl a
inc a
inc a
tpl a
tpl a
tpl a
inc a
jmp +23
tpl a
inc a
inc a
tpl a
inc a
inc a
tpl a
tpl a
inc a
inc a
tpl a
inc a
tpl a
inc a
tpl a
inc a
inc a
tpl a
inc a
tpl a
tpl a
inc a
jio a, +8
inc b
jie a, +4
tpl a
inc a
jmp +2
hlf a
jmp -7)"};
}
