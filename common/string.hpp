#pragma once
#include <string>
#include <string_view>
#include <vector>

[[nodiscard]] std::vector<std::string> Split(std::string_view text, char sep);
void PrintUnicode(std::wstring_view text);
