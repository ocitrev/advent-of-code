#pragma once
#include <filesystem>

std::filesystem::path const & GetExePath();
#pragma comment(linker, "/INCLUDE:wmain")
