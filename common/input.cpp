#include "pch.h"
#include "input.hpp"
#include "main.hpp"

#include <fstream>
#include <charconv>
#include <string>

std::filesystem::path GetInputsPath()
{
    return std::filesystem::absolute(GetExePath().parent_path() / L"..\\..\\inputs");
}

std::vector<int> ReadInt32s(std::filesystem::path const &filepath, char sep)
{
    std::ifstream input(filepath);
    std::vector<int> results;

    for (std::string line; std::getline(input, line, sep); )
    {
        int value;
        auto result = std::from_chars(line.data(), line.data() + line.size(), value, 10);

        if (result.ec == std::errc{})
            results.push_back(value);
    }

    return results;
}

std::vector<long long> ReadInt64s(std::filesystem::path const &filepath, char sep)
{
    std::ifstream input(filepath);
    std::vector<long long> results;

    for (std::string line; std::getline(input, line, sep); )
    {
        int value;
        auto result = std::from_chars(line.data(), line.data() + line.size(), value, 10);

        if (result.ec == std::errc{})
            results.push_back(value);
    }

    return results;
}

std::string ReadAllText(std::filesystem::path const &filepath)
{
    std::ifstream input(filepath, std::ios_base::ate | std::ios_base::binary);
    auto size = input.tellg();
    input.seekg(0);

    std::string ret(static_cast<size_t>(size), '\0');
    input.read(ret.data(), size);
    trim(ret);
    return ret;
}
