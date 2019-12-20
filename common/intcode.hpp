#pragma once

#include <vector>
#include <functional>
#include <optional>
#include <filesystem>

using Int = long long;

class Intcode
{
    using InputFunc = std::function<Int()>;
    using OutputFunc = std::function<void (Int)>;

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
    [[nodiscard]] std::optional<Int> RunUntilOuput(InputFunc &&inputFunc);

    static void Run(std::vector<Int> code);
    static void Run(std::vector<Int> code, InputFunc &&inputFunc, OutputFunc &&outputFunc);

    [[nodiscard]] Int ReadMemory(std::size_t offset) const;
    Int WriteMemory(std::size_t offset, Int value);

    void SetInput(InputFunc const &inputFunc);
    void SetOutput(OutputFunc const &outputFunc);

    [[nodiscard]] bool IsHalted() const
    {
        return halted;
    }

    static std::vector<Int> ReadFile(std::filesystem::path const &filepath);

};
