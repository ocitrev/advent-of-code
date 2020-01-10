#include "common.hpp"
#include "intcode.hpp"
#include "input.hpp"
#include <stdexcept>
#include <sstream>


Intcode::Intcode(std::vector<Int> code)
    : memory{std::move(code)}
{
    if (memory.empty())
        throw std::invalid_argument("code cannot be empty.");
}

Intcode::Param Intcode::GetParam(Mode mode)
{
    return {memory[ip++], this, mode};
}

Intcode::Param &Intcode::Param::operator=(Int value_)
{
    if (mode == Mode::Immediate)
        throw std::domain_error("Cannot set immediate value");

    Int offset = value;

    if (mode == Mode::Relative)
        offset += cpu->relOffset;

    if (offset >= static_cast<Int>(cpu->memory.size()))
        cpu->memory.resize(static_cast<size_t>(offset) + 1);

    cpu->memory[static_cast<size_t>(offset)] = value_;
    return *this;
}

Intcode::Param::operator Int() const
{
    if (mode == Mode::Immediate)
        return value;

    Int offset = value;

    if (mode == Mode::Relative)
        offset += cpu->relOffset;

    if (offset >= static_cast<Int>(cpu->memory.size()))
        return 0;

    return cpu->memory[static_cast<size_t>(offset)];
}

std::pair<typename Intcode::OpCode, Int> Intcode::GetInstruction()
{
    Int instruction = memory[ip++];
    return {static_cast<OpCode>(instruction % 100), instruction / 100};
}

void Intcode::Run(std::vector<Int> code)
{
    Intcode a{std::move(code)};
    a.Run();
}

void Intcode::Run(std::vector<Int> code, InputFunc &&inputFunc, OutputFunc &&outputFunc)
{
    Intcode a{std::move(code)};
    a.SetInput(std::move(inputFunc));
    a.SetOutput(std::move(outputFunc));
    a.Run();
}

void Intcode::RunStep(Intcode::OpCode opcode, Int mode)
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
            a = inputFunc_();
            break;
        }
        case OpCode::Output:
        {
            auto a = GetParam(static_cast<Mode>(mode % 10));
            outputFunc_(a);
            break;
        }
        case OpCode::JumpTrue:
        {
            auto a = GetParam(static_cast<Mode>(mode % 10));
            auto b = GetParam(static_cast<Mode>((mode / 10) % 10));

            if (a != 0)
            {
                ip = static_cast<size_t>(b);
            }
            break;
        }
        case OpCode::JumpFalse:
        {
            auto a = GetParam(static_cast<Mode>(mode % 10));
            auto b = GetParam(static_cast<Mode>((mode / 10) % 10));

            if (a == 0)
            {
                ip = static_cast<size_t>(b);
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

std::function<void (Int)> Intcode::RunUntilInput()
{
    while (halted == false)
    {
        auto [opcode, mode] = GetInstruction();

        if (opcode == OpCode::Input)
        {
            auto a = GetParam(static_cast<Mode>(mode % 10));

            return [a](Int value) mutable
            {
                a = value;
            };
        }

        RunStep(opcode, mode);
    }

    return nullptr;
}

std::optional<Int> Intcode::RunUntilOuput(InputFunc &&inputFunc)
{
    inputFunc_ = std::move(inputFunc);

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

Int Intcode::ReadMemory(std::size_t offset) const
{
    return memory.at(offset);
}

Int Intcode::WriteMemory(size_t offset, Int value)
{
    return std::exchange(memory.at(offset), value);
}

void Intcode::SetInput(InputFunc const &inputFunc)
{
    inputFunc_ = inputFunc;
}

void Intcode::SetOutput(OutputFunc const &outputFunc)
{
    outputFunc_ = outputFunc;
}

std::vector<Int> Intcode::ReadFile(std::filesystem::path const &filepath)
{
    return ReadInt64s(GetInputsPath() / filepath, ',');
}

std::vector<Int> Intcode::ReadString(std::string_view data)
{
    std::istringstream ss({data.data(), data.size()});
    return ReadInt64s(ss, ',');
}
