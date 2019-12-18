#include "pch.h"
#include "intcode.hpp"
#include <stdexcept>

Intcode::Intcode(std::vector<int> code)
    : memory{std::move(code)}
{
    if (memory.empty())
        throw std::invalid_argument("code cannot be empty.");
}

Intcode::Param Intcode::GetParam(Mode mode)
{
    return {memory[ip++], this, mode};
}

Intcode::Param &Intcode::Param::operator=(int value)
{
    if (mode == Mode::Immediate)
        throw std::domain_error("Cannot set immediate value");

    cpu->memory[this->value] = value;
    return *this;
}

Intcode::Param::operator int() const
{
    if (mode == Mode::Immediate)
        return value;

    return cpu->memory[value];
}

std::pair<Intcode::OpCode, int> Intcode::GetInstruction()
{
    int instruction = memory[ip++];
    return {static_cast<OpCode>(instruction % 100), instruction / 100};
}

void Intcode::Run(std::vector<int> code)
{
    Intcode a{std::move(code)};
    a.Run();
}

void Intcode::Run(std::vector<int> code, std::function<int()> &&inputFunc, std::function<void(int)> &&outputFunc)
{
    Intcode a{std::move(code)};
    a.SetInput(std::move(inputFunc));
    a.SetOutput(std::move(outputFunc));
    a.Run();
}

void Intcode::RunStep(Intcode::OpCode opcode, int mode)
{
    switch (opcode)
    {
        case OpCode::Add:
        {
            auto a = GetParam(static_cast<Mode>(mode % 10));
            auto b = GetParam(static_cast<Mode>((mode / 10) % 10));
            auto c = GetParam(static_cast<Mode>((mode / 100) % 10));
            c = a + b;
            break;
        }
        case OpCode::Muliply:
        {
            auto a = GetParam(static_cast<Mode>(mode % 10));
            auto b = GetParam(static_cast<Mode>((mode / 10) % 10));
            auto c = GetParam(static_cast<Mode>((mode / 100) % 10));
            c = a * b;
            break;
        }
        case OpCode::Input:
        {
            auto a = GetParam(static_cast<Mode>(mode % 10));
            a = inputFunc();
            break;
        }
        case OpCode::Output:
        {
            auto a = GetParam(static_cast<Mode>(mode % 10));
            outputFunc(a);
            break;
        }
        case OpCode::JumpTrue:
        {
            auto a = GetParam(static_cast<Mode>(mode % 10));
            auto b = GetParam(static_cast<Mode>((mode / 10) % 10));

            if (a != 0)
            {
                ip = b;
            }
            break;
        }
        case OpCode::JumpFalse:
        {
            auto a = GetParam(static_cast<Mode>(mode % 10));
            auto b = GetParam(static_cast<Mode>((mode / 10) % 10));

            if (a == 0)
            {
                ip = b;
            }
            break;
        }
        case OpCode::IsLess:
        {
            auto a = GetParam(static_cast<Mode>(mode % 10));
            auto b = GetParam(static_cast<Mode>((mode / 10) % 10));
            auto c = GetParam(static_cast<Mode>((mode / 100) % 10));
            c = a < b;
            break;
        }
        case OpCode::IsEqual:
        {
            auto a = GetParam(static_cast<Mode>(mode % 10));
            auto b = GetParam(static_cast<Mode>((mode / 10) % 10));
            auto c = GetParam(static_cast<Mode>((mode / 100) % 10));
            c = a == b;
            break;
        }
        case OpCode::SetRelBaseOffset:
        {
            auto a = GetParam(static_cast<Mode>(mode % 10));
            relOffset += a;
            break;
        }
        case OpCode::Halt:
            halted = true;
            break;
        default:
            throw std::domain_error("Invalid opcode");
    }
}

void Intcode::Run()
{
    while (halted == false)
    {
        auto [opcode, mode] = GetInstruction();
        RunStep(opcode, mode);
    }
}

std::optional<int> Intcode::RunUntilOuput(std::function<int()> &&inputFunc)
{
    this->inputFunc = std::move(inputFunc);

    while (halted == false)
    {
        auto [opcode, mode] = GetInstruction();

        if (opcode == OpCode::Output)
        {
            auto a = GetParam(static_cast<Mode>(mode % 10));
            return a;
        }

        RunStep(opcode, mode);
    }

    return std::nullopt;
}

int Intcode::ReadMemory(std::size_t offset) const
{
    return memory.at(offset);
}

int Intcode::WriteMemory(size_t offset, int value)
{
    return std::exchange(memory.at(offset), value);
}

void Intcode::SetInput(std::function<int()> const &inputFunc)
{
    this->inputFunc = inputFunc;
}

void Intcode::SetOutput(std::function<void(int)> const &outputFunc)
{
    this->outputFunc = outputFunc;
}
