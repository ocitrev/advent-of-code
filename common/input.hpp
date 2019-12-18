#pragma once

#include <vector>
#include <filesystem>

[[nodiscard]]
std::filesystem::path GetInputsPath();

[[nodiscard]]
std::vector<int> ReadInts(std::filesystem::path const &filepath, char sep);
