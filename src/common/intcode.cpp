#include "intcode.hpp"
#include <sstream>
#include <stdexcept>

struct Intcode::Param
{
    Int value;
    Intcode *cpu;
    Mode mode;
    Param &operator=(Int value);
    operator Int() const;
};

Intcode::Intcode(std::vector<Int> code)
    : state{std::move(code)}
{
    if (state.memory.empty())
        throw std::invalid_argument("code cannot be empty.");
}

Intcode::Param Intcode::GetParam(Mode mode)
{
    return {state.memory[state.ip++], this, mode};
}

Intcode::Param &Intcode::Param::operator=(Int value_)
{
    if (mode == Mode::Immediate)
        throw std::domain_error("Cannot set immediate value");

    Int offset = value;

    if (mode == Mode::Relative)
        offset += cpu->state.relOffset;

    if (offset >= static_cast<Int>(cpu->state.memory.size()))
        cpu->state.memory.resize(static_cast<size_t>(offset) + 1);

    cpu->state.memory[static_cast<size_t>(offset)] = value_;
    return *this;
}

Intcode::Param::operator Int() const
{
    if (mode == Mode::Immediate)
        return value;

    Int offset = value;

    if (mode == Mode::Relative)
        offset += cpu->state.relOffset;

    if (offset >= static_cast<Int>(cpu->state.memory.size()))
        return 0;

    return cpu->state.memory[static_cast<size_t>(offset)];
}

std::pair<typename Intcode::OpCode, Int> Intcode::GetInstruction()
{
    Int instruction = state.memory[state.ip++];
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
            state.ip = static_cast<size_t>(b);

        break;
    }
    case OpCode::JumpFalse:
    {
        auto a = GetParam(static_cast<Mode>(mode % 10));
        auto b = GetParam(static_cast<Mode>((mode / 10) % 10));

        if (a == 0)
            state.ip = static_cast<size_t>(b);

        break;
    }
    case OpCode::IsLess:
    {
        auto a = GetParam(static_cast<Mode>(mode % 10));
        auto b = GetParam(static_cast<Mode>((mode / 10) % 10));
        auto c = GetParam(static_cast<Mode>((mode / 100) % 10));
        c = a < b ? 1 : 0;
        break;
    }
    case OpCode::IsEqual:
    {
        auto a = GetParam(static_cast<Mode>(mode % 10));
        auto b = GetParam(static_cast<Mode>((mode / 10) % 10));
        auto c = GetParam(static_cast<Mode>((mode / 100) % 10));
        c = a == b ? 1 : 0;
        break;
    }
    case OpCode::SetRelBaseOffset:
    {
        auto a = GetParam(static_cast<Mode>(mode % 10));
        state.relOffset += a;
        break;
    }
    case OpCode::Halt:
        state.halted = true;
        break;
    default:
        throw std::domain_error("Invalid opcode");
    }
}

void Intcode::Run()
{
    while (!state.halted)
    {
        auto [opcode, mode] = GetInstruction();
        RunStep(opcode, mode);
    }
}

Intcode::OutputFunc Intcode::RunUntilInput()
{
    while (!state.halted)
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

    while (!state.halted)
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
    return state.memory.at(offset);
}

Int Intcode::WriteMemory(size_t offset, Int value)
{
    return std::exchange(state.memory[offset], value);
}

void Intcode::SetInput(InputFunc &&inputFunc)
{
    inputFunc_ = std::move(inputFunc);
}

void Intcode::SetOutput(OutputFunc &&outputFunc)
{
    outputFunc_ = std::move(outputFunc);
}

Intcode::State Intcode::Backup() const
{
    return state;
}

void Intcode::Restore(Intcode::State const &backup)
{
    state = backup;
}
