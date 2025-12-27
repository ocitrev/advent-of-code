#include "day8.hpp"

#include "../common/assert.hpp"
#include "../common/string.hpp"

#include <print>
#include <set>
#include <span>
#include <utility>
#include <vector>

struct VM
{
    std::span<std::string_view const> instructions;

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
            {
                nb = -nb;
            }
            acc += nb;
            ++ip;
        }
        else if (starts_with(ins, "jmp"))
        {
            auto nb = svtoi(ins.substr(5));
            if (ins[4] == '-')
            {
                nb = -nb;
            }
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

static int Run1(std::span<std::string_view const> instructions)
{
    VM vm;
    vm.instructions = instructions;
    vm.Run();
    return vm.acc;
}

static int Run1(std::string_view instructions)
{
    auto const lines = Split(instructions, '\n');
    return Run1(lines);
}

static int Run2(std::span<std::string_view const> instructions)
{
    std::vector<std::string_view> copy(begin(instructions), end(instructions));

    auto first = begin(copy);
    auto last = end(copy);

    while (true)
    {
        first = std::find_if(first, last,
            [](std::string_view const &i)
            {
                return starts_with(i, "jmp");
            });

        if (first == last)
        {
            break;
        }

        auto old = std::exchange(*first, "nop +0");

        VM vm;
        vm.instructions = copy;
        vm.Run();
        if (vm.HasTerminated())
        {
            return vm.acc;
        }

        *first = old;
        ++first;
    }

    first = begin(copy);

    while (true)
    {
        first = std::find_if(first, last,
            [](std::string_view const &i)
            {
                return starts_with(i, "nop");
            });

        if (first == last)
        {
            break;
        }

        std::string jump{*first};
        jump.replace(0, 3, "jmp");
        auto old = std::exchange(*first, jump);

        VM vm;
        vm.instructions = copy;
        vm.Run();
        if (vm.HasTerminated())
        {
            return vm.acc;
        }

        *first = old;
        ++first;
    }

    return 0;
}

static int Run2(std::string_view instructions)
{
    auto const lines = Split(instructions, '\n');
    return Run2(lines);
}

int main()
{
    // https://adventofcode.com/2020/day/8
    std::println("Day 8, 2020: ");

    Assert(5 == Run1(example::instructions));
    Assert(8 == Run2(example::instructions));

    auto const part1 = Run1(GetInput());
    std::println("  Part 1: {}", part1);
    Assert(1528 == part1);

    auto const part2 = Run2(GetInput());
    std::println("  Part 2: {}", part2);
    Assert(640 == part2);
}
