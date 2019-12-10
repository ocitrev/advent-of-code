#include "pch.h"
#include "intcode.hpp"
#include <stdexcept>

Intcode::Intcode(std::vector<int> code)
    : memory{std::move(code)}
{

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

std::pair<int, int> Intcode::GetInstruction()
{
    int instruction = memory[ip++];
    return {instruction % 100, instruction / 100};
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


void Intcode::Run()
{
    while (true)
    {
        auto [opcode, mode] = GetInstruction();

        switch (opcode)
        {
            case 1:
            {
                auto a = GetParam(static_cast<Mode>(mode % 10));
                auto b = GetParam(static_cast<Mode>((mode / 10) % 10));
                auto c = GetParam(static_cast<Mode>((mode / 100) % 10));
                c = a + b;
                break;
            }
            case 2:
            {
                auto a = GetParam(static_cast<Mode>(mode % 10));
                auto b = GetParam(static_cast<Mode>((mode / 10) % 10));
                auto c = GetParam(static_cast<Mode>((mode / 100) % 10));
                c = a * b;
                break;
            }
            case 3:
            {
                auto a = GetParam(static_cast<Mode>(mode % 10));
                a = inputFunc();
                break;
            }
            case 4:
            {
                auto a = GetParam(static_cast<Mode>(mode % 10));
                outputFunc(a);
                break;
            }
            case 5:
            {
                auto a = GetParam(static_cast<Mode>(mode % 10));
                auto b = GetParam(static_cast<Mode>((mode / 10) % 10));

                if (a != 0)
                {
                    ip = b;
                }
                break;
            }
            case 6:
            {
                auto a = GetParam(static_cast<Mode>(mode % 10));
                auto b = GetParam(static_cast<Mode>((mode / 10) % 10));

                if (a == 0)
                {
                    ip = b;
                }
                break;
            }
            case 7:
            {
                auto a = GetParam(static_cast<Mode>(mode % 10));
                auto b = GetParam(static_cast<Mode>((mode / 10) % 10));
                auto c = GetParam(static_cast<Mode>((mode / 100) % 10));
                c = a < b;
                break;
            }
            case 8:
            {
                auto a = GetParam(static_cast<Mode>(mode % 10));
                auto b = GetParam(static_cast<Mode>((mode / 10) % 10));
                auto c = GetParam(static_cast<Mode>((mode / 100) % 10));
                c = a == b;
                break;
            }
            case 9:
            {
                auto a = GetParam(static_cast<Mode>(mode % 10));
                relOffset += a;
                break;
            }
            case 99:
                return;
        }
    }
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
