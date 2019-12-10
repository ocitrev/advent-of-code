#pragma once

#include <vector>
#include <functional>

class Intcode
{
    std::vector<int> memory;
    std::size_t ip = 0;
    int relOffset = 0;
    std::function<int()> inputFunc;
    std::function<void(int)> outputFunc;

    enum class Mode
    {
        Address,
        Immediate,
        Relative,
    };

    struct Param
    {
        int value;
        Intcode *cpu;
        Mode mode;
        Param &operator=(int value);
        operator int() const;
    };

    Param GetParam(Mode mode);
    std::pair<int, int> GetInstruction();


public:
    explicit Intcode(std::vector<int> code);
    void Run();

    static void Run(std::vector<int> code);
    static void Run(std::vector<int> code, std::function<int()> &&inputFunc, std::function<void(int)> &&outputFunc);

    int ReadMemory(std::size_t offset) const;
    int WriteMemory(std::size_t offset, int value);

    void SetInput(std::function<int()> const &inputFunc);
    void SetOutput(std::function<void(int)> const &outputFunc);
};