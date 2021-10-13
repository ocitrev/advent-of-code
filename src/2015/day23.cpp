#include "day23.hpp"
#include "../common/assert.hpp"
#include "../common/string.hpp"
#include <fmt/format.h>
#include <gsl/gsl>

struct VM
{
    unsigned int a = 0;
    unsigned int b = 0;
    int ip = 0;

    gsl::span<std::string_view const> instructions;

    unsigned int &GetRegister(std::string_view instruction)
    {
        if (instruction[4] == 'a')
            return a;

        if (instruction[4] == 'b')
            return b;

        throw std::runtime_error("invalid register");
    }

    void Jump(std::string_view offsetText)
    {
        int const offset = svtoi(offsetText.substr(1));
        if (offsetText[0] == '-')
            ip -= offset;
        else
            ip += offset;
    }

    void Step()
    {
        auto const instruction = instructions[static_cast<std::size_t>(ip)];

        if (starts_with(instruction, "hlf"))
        {
            GetRegister(instruction) /= 2;
            ++ip;
        }
        else if (starts_with(instruction, "tpl"))
        {
            GetRegister(instruction) *= 3;
            ++ip;
        }
        else if (starts_with(instruction, "inc"))
        {
            ++GetRegister(instruction);
            ++ip;
        }
        else if (starts_with(instruction, "jmp"))
        {
            Jump(instruction.substr(4));
        }
        else if (starts_with(instruction, "jie"))
        {
            if (GetRegister(instruction) % 2 == 0)
                Jump(instruction.substr(7));
            else
                ++ip;
        }
        else if (starts_with(instruction, "jio"))
        {
            if (GetRegister(instruction) == 1)
                Jump(instruction.substr(7));
            else
                ++ip;
        }
        else
        {
            throw std::runtime_error("Invalid instruction");
        }
    }

    void Run()
    {
        while (static_cast<std::size_t>(ip) < instructions.size())
        {
            Step();
        }
    }
};

auto Example()
{
    using namespace std::string_view_literals;
    VM vm;
    vm.instructions = std::array{
        "inc a"sv,
        "jio a, +2"sv,
        "tpl a"sv,
        "inc a"sv,
    };

    vm.Run();
    return vm.a;
}

auto Part1()
{
    VM vm;
    vm.instructions = input::instructions;
    vm.Run();
    return vm.b;
}

auto Part2()
{
    VM vm;
    vm.instructions = input::instructions;
    vm.a = 1;
    vm.Run();
    return vm.b;
}

int main()
{
    // https://adventofcode.com/2015/day/23
    fmt::print("Day 23, 2015: Opening the Turing Lock\n");

    Assert(2 == Example());

    auto const part1 = Part1();
    fmt::print("  Part 1: {}\n", part1);
    Assert(170 == part1);

    auto const part2 = Part2();
    fmt::print("  Part 2: {}\n", part2);
    Assert(247 == part2);
}
