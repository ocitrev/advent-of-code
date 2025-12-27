#include "day14.hpp"

#include "../common.hpp"

#include <bitset>

struct VM
{
    inline static constexpr size_t kNbBits = 36;
    std::unordered_map<uint64_t, uint64_t> memory;
    std::bitset<kNbBits> maskOnes;
    std::bitset<kNbBits> maskValue{0xffffffffffffffff};
    std::string_view lastMask;

    void ParseMask(std::string_view mask)
    {
        lastMask = mask;
        std::string mv{mask};
        for (char &c : mv)
        {
            if (c == 'X')
            {
                c = '1';
            }
            else
            {
                c = '0';
            }
        }

        std::string mo{mask};
        for (char &c : mo)
        {
            if (c != '1')
            {
                c = '0';
            }
        }

        maskValue = std::bitset<kNbBits>(mv);
        maskOnes = std::bitset<kNbBits>(mo);
    }

    void RunInstruction(std::string_view instruction)
    {
        if (starts_with(instruction, "mask"))
        {
            ParseMask(instruction.substr(7));
        }
        else if (starts_with(instruction, "mem"))
        {
            auto const address = svtoi<size_t>(instruction.substr(4));
            auto const value = svtoi<uint64_t>(instruction.substr(instruction.find('=') + 2));
            auto const bits = (std::bitset<kNbBits>{value} & maskValue) | maskOnes;
            memory[address] = bits.to_ullong();
        }
    }

    void RunInstruction2(std::string_view instruction)
    {
        if (starts_with(instruction, "mask"))
        {
            ParseMask(instruction.substr(7));
        }
        else if (starts_with(instruction, "mem"))
        {
            auto const address = svtoi<uint64_t>(instruction.substr(4));
            auto const value = svtoi<uint64_t>(instruction.substr(instruction.find('=') + 2));

            for (auto m : GenerateAllBits(lastMask, address))
            {
                memory[m] = value;
            }
        }
    }

    [[nodiscard]] uint64_t Sum() const
    {
        return std::accumulate(begin(memory), end(memory), uint64_t{0},
            [](uint64_t total, auto const &item)
            {
                return total + item.second;
            });
    }

    [[nodiscard]] static std::vector<uint64_t> GenerateAllBits(std::string_view mask, uint64_t initial)
    {
        std::vector<uint64_t> masks;
        auto const bitCount = static_cast<int>(std::count(begin(mask), end(mask), 'X'));
        int const max = 1 << bitCount;
        std::string const initialBits = std::bitset<kNbBits>{initial}.to_string();

        for (int n = 0; n < max; ++n)
        {
            auto bits = std::format("{:0{}b}\n", n, bitCount);

            std::string bitMask(mask);
            auto iter = begin(bits);

            for (size_t bit = 0; bit < kNbBits; ++bit)
            {
                auto &c = bitMask[bit];
                if (c == 'X')
                {
                    c = *iter++;
                }
                else if (initialBits[bit] == '1')
                {
                    c = '1';
                }
            }

            masks.emplace_back(std::bitset<kNbBits>{bitMask}.to_ullong());
        }

        return masks;
    }
};

static uint64_t Part1()
{
    VM vm;
    for (auto &&ins : Split(GetInput(), '\n'))
    {
        vm.RunInstruction(ins);
    }

    return vm.Sum();
}

static uint64_t Part2()
{
    VM vm;
    for (auto &&ins : Split(GetInput(), '\n'))
    {
        vm.RunInstruction2(ins);
    }

    return vm.Sum();
}

static void Example()
{
#ifndef NDEBUG
    VM vm;
    for (auto &&ins : example::instructions)
    {
        vm.RunInstruction2(ins);
    }

    Assert(208 == vm.Sum());
#endif
}

int main()
{
    // https://adventofcode.com/2020/day/14
    std::println("Day 14, 2020: Docking Data");

    Example();

    auto const part1 = Part1();
    std::println("  Part 1: {}", part1);
    Assert(8'471'403'462'063 == part1);

    auto const part2 = Part2();
    std::println("  Part 2: {}", part2);
    Assert(2'667'858'637'669 == part2);
}
