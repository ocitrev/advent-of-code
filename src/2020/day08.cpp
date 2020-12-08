#include "day08.hpp"
#include "../common/assert.hpp"
#include "../common/string.hpp"
#include <fmt/format.h>
#include <gsl/gsl>
#include <set>
#include <vector>

struct VM
{
    gsl::span<std::string_view const> instructions;

    int ip = 0;
    int acc = 0;
    std::set<int> ran;

    void Step()
    {
        using namespace std::string_view_literals;
        auto ins = instructions[static_cast<size_t>(ip)];
        ran.insert(ip);

        if (starts_with(ins, "nop"sv))
        {
            ++ip;
        }
        else if (starts_with(ins, "acc"))
        {
            auto nb = svtoi(ins.substr(5));
            if (ins[4] == '-')
                nb = -nb;
            acc += nb;
            ++ip;
        }
        else if (starts_with(ins, "jmp"))
        {
            auto nb = svtoi(ins.substr(5));
            if (ins[4] == '-')
                nb = -nb;
            ip += nb;
        }
    }

    void Run()
    {
        while (ran.find(ip) == end(ran) && not HasTerminated())
        {
            Step();
        }
    }

    [[nodiscard]] bool HasTerminated() const
    {
        return static_cast<size_t>(ip) >= instructions.size();
    }
};

int Run1(gsl::span<std::string_view const> instructions)
{
    VM vm;
    vm.instructions = instructions;
    vm.Run();
    return vm.acc;
}

int Run2(gsl::span<std::string_view const> instructions)
{
    std::vector<std::string_view> copy(begin(instructions), end(instructions));

    auto first = begin(copy);
    auto last = end(copy);

    while (true)
    {
        first = std::find_if(first, last, [](std::string_view const &i) {
            return starts_with(i, "jmp");
        });

        if (first == last)
            break;

        auto old = std::exchange(*first, "nop +0");

        VM vm;
        vm.instructions = copy;
        vm.Run();
        if (vm.HasTerminated())
            return vm.acc;

        *first = old;
        ++first;
    }

    first = begin(copy);

    while (true)
    {
        first = std::find_if(first, last, [](std::string_view const &i) {
            return starts_with(i, "nop");
        });

        if (first == last)
            break;

        std::string jump{*first};
        jump.replace(0, 3, "jmp");
        auto old = std::exchange(*first, jump);

        VM vm;
        vm.instructions = copy;
        vm.Run();
        if (vm.HasTerminated())
            return vm.acc;

        *first = old;
        ++first;
    }

    return 0;
}

int main()
{
    // https://adventofcode.com/2020/day/8
    fmt::print("Day 8: \n");

    Assert(5 == Run1(example::instructions));
    Assert(8 == Run2(example::instructions));

    auto const part1 = Run1(input::instructions);
    fmt::print("  Part 1: {}\n", part1);
    Assert(1528 == part1);

    auto const part2 = Run2(input::instructions);
    fmt::print("  Part 2: {}\n", part2);
    Assert(640 == part2);
}
