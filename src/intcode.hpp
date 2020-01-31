#pragma once

#include <functional>
#include <optional>
#include <vector>

using Int = long long;

class Intcode
{
    using InputFunc = std::function<Int()>;
    using OutputFunc = std::function<void(Int)>;

    enum class Mode
    {
        Address = 0,
        Immediate = 1,
        Relative = 2,
    };

    enum class OpCode
    {
        Invalid = 0,
        Add = 1,
        Muliply = 2,
        Input = 3,
        Output = 4,
        JumpTrue = 5,
        JumpFalse = 6,
        IsLess = 7,
        IsEqual = 8,
        SetRelBaseOffset = 9,
        Halt = 99,
    };

    struct Param
    {
        Int value;
        Intcode *cpu;
        Mode mode;
        Param &operator=(Int value);
        operator Int() const;
    };

    std::vector<Int> memory;
    std::size_t ip = 0;
    Int relOffset = 0;
    bool halted = false;
    InputFunc inputFunc_;
    OutputFunc outputFunc_;

    Param GetParam(Mode mode);
    std::pair<OpCode, Int> GetInstruction();

    void RunStep(OpCode opcode, Int mode);

public:
    explicit Intcode(std::vector<Int> code);
    void Run();
    OutputFunc RunUntilInput();
    [[nodiscard]] std::optional<Int> RunUntilOuput(InputFunc &&inputFunc);

    static void Run(std::vector<Int> code);
    static void Run(std::vector<Int> code, InputFunc &&inputFunc, OutputFunc &&outputFunc);

    [[nodiscard]] Int ReadMemory(std::size_t offset) const;
    Int WriteMemory(std::size_t offset, Int value);

    void SetInput(InputFunc &&inputFunc);
    void SetOutput(OutputFunc &&outputFunc);

    [[nodiscard]] bool IsHalted() const
    {
        return halted;
    }
};
