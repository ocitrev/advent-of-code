param(
    [int]$Day = [datetime]::Now.AddHours(+1).Day,
    [int]$Year = [datetime]::Now.Year
)

$cpp = @"
#include "day$Day.hpp"
#include "../common.hpp"

static auto Part1()
{
    return 0;
}

static auto Part2()
{
    return 0;
}

int main()
{
    // https://adventofcode.com/$Year/day/$Day
    fmt::print("Day $Day, ${Year}: \n");

    auto const part1 = Part1();
    fmt::print("  Part 1: {}\n", part1);
    // Assert( == part1);

    auto const part2 = Part2();
    fmt::print("  Part 2: {}\n", part2);
    // Assert( == part2);
}
"@

$hpp = @"
#pragma once
#include "input-bytes.hpp"
#include <string_view>

namespace example
{
    using namespace std::literals;
    static constexpr auto text = R"()"sv;
}
"@

$cppFilepath = (Join-Path $PSScriptRoot "Src\$Year\day$Day.cpp")
$hppFilepath = (Join-Path $PSScriptRoot "Src\$Year\day$Day.hpp")

if (Test-Path $cppFilepath)
{
    Write-Warning "$Year\day$Day.cpp already exists"
}
elseif (Test-Path $hppFilepath)
{
    Write-Warning "$Year\day$Day.hpp already exists"
}
else
{
    Set-Content -Path (Join-Path $PSScriptRoot "Src\$Year\day$Day.cpp") -Value $cpp -Encoding utf8
    Set-Content -Path (Join-Path $PSScriptRoot "Src\$Year\day$Day.hpp") -Value $hpp -Encoding utf8
    Set-Content -Path (Join-Path $PSScriptRoot "inputs\$Year\day$Day.txt") -Value "" -NoNewline -Encoding utf8
    Add-Content -Path (Join-Path $PSScriptRoot 'CMakeLists.txt') -Value "add_aoc($Year $Day)" -Encoding utf8
}

