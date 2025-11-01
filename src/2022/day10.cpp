#include "day10.hpp"

#include "../common.hpp"

using namespace std::literals;

enum struct OpCode
{
    Invalid,
    AddX,
    Noop,
};

static OpCode ParseOpCode(std::string_view text)
{
    if (text == "addx"sv)
    {
        return OpCode::AddX;
    }

    if (text == "noop"sv)
    {
        return OpCode::Noop;
    }

    return OpCode::Invalid;
}

struct CPU
{
    int X = 1;
    int cycleCount = 0;
    int score = 0;
    std::vector<std::string> lines;

    void Cycle()
    {
        Render();
        ++cycleCount;
        ComputeSignalStrength();
    }

    void ComputeSignalStrength()
    {
        switch (cycleCount)
        {
            case 20:
            case 60:
            case 100:
            case 140:
            case 180:
            case 220:
                score += cycleCount * X;
                break;
        }
    }

    void Render()
    {
        auto const pixel = cycleCount % 40;

        if (pixel == 0)
        {
            lines.emplace_back().reserve(40);
        }

        if (X - 1 == pixel || pixel == X || pixel == X + 1)
        {
            lines.back().append("█"sv);
        }
        else
        {
            lines.back().append(1, ' ');
        }
    }

    void Step(std::string_view instr)
    {
        auto operands = Split(instr, ' ');
        auto const op = ParseOpCode(operands[0]);

        switch (op)
        {
            case OpCode::AddX:
                Cycle();
                Cycle();
                X += svtoi(operands[1]);
                break;

            case OpCode::Noop:
                Cycle();
                break;

            default:
                throw std::invalid_argument("instr");
        }
    }

    CPU &Run(std::string_view instructions)
    {
        for (auto instr : Split(instructions, '\n'))
        {
            Step(instr);
        }

        return *this;
    }

    int GetScore() const
    {
        return score;
    }

    int GetX() const
    {
        return X;
    }

    std::string Print() const
    {
        return Join(begin(lines), end(lines), "\n"sv);
    }
};

static int RunProgram(std::string_view instructions)
{
    CPU cpu;
    cpu.Run(instructions);
    return cpu.GetScore();
}

static std::string PrintProgram(std::string_view instructions)
{
    CPU cpu;
    cpu.Run(instructions);
    return cpu.Print();
}

static int Part1()
{
    return RunProgram(GetInput());
}

static auto Part2()
{
    return PrintProgram(GetInput());
}

int main()
{
    // https://adventofcode.com/2022/day/10
    fmt::print("Day 10, 2022: Cathode-Ray Tube\n");

    Assert(-1 == CPU{}.Run(example::small_program).GetX());
    Assert(13'140 == RunProgram(example::large_program));
    Assert(example::expected == PrintProgram(example::large_program));

    auto const part1 = Part1();
    fmt::print("  Part 1: {}\n", part1);
    Assert(11'220 == part1);

    auto const part2 = Part2();
    fmt::print("  Part 2:\n{}\n", part2);
    Assert(input::expected == part2);
}
