#pragma once

#include <vector>
#include <functional>
#include <optional>

class Intcode
{
    enum class Mode
    {
        Address,
        Immediate,
        Relative,
    };

    enum class OpCode
    {
        Invalid,
        Add,
        Muliply,
        Input,
        Output,
        JumpTrue,
        JumpFalse,
        IsLess,
        IsEqual,
        SetRelBaseOffset,
        Halt = 99,
    };

    struct Param
    {
        int value;
        Intcode *cpu;
        Mode mode;
        Param &operator=(int value);
        operator int() const;
    };

    std::vector<int> memory;
    std::size_t ip = 0;
    int relOffset = 0;
    bool halted = false;
    std::function<int()> inputFunc_;
    std::function<void(int)> outputFunc_;

    Param GetParam(Mode mode);
    std::pair<OpCode, int> GetInstruction();

    void RunStep(OpCode opcode, int mode);

public:
    explicit Intcode(std::vector<int> code);
    void Run();
    [[nodiscard]] std::optional<int> RunUntilOuput(std::function<int()> &&inputFunc);

    static void Run(std::vector<int> code);
    static void Run(std::vector<int> code, std::function<int()> &&inputFunc, std::function<void(int)> &&outputFunc);

    [[nodiscard]] int ReadMemory(std::size_t offset) const;
    int WriteMemory(std::size_t offset, int value);

    void SetInput(std::function<int()> const &inputFunc);
    void SetOutput(std::function<void(int)> const &outputFunc);

    [[nodiscard]] bool IsHalted() const
    {
        return halted;
    }

};