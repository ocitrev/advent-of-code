param(
    [int]$Day = [datetime]::Now.AddHours(+1).Day,
    [int]$Year = [datetime]::Now.Year,
    [switch]$Zig
)

if ($Zig.IsPresent)
{
    $zigText = @"
const std = @import("std");

fn part1(input: []const u8, allocator: std.mem.Allocator) !i32 {
    _ = allocator;
    _ = input;
    return 0;
}

test "part 1" {
    const example =
        \\
    ;
    try std.testing.expectEqual(@as(i32, 0), try part1(example, std.testing.allocator));
}

fn part2(input: []const u8, allocator: std.mem.Allocator) !i32 {
    _ = allocator;
    _ = input;
    return 0;
}

test "part 2" {
    const example =
        \\
    ;
    try std.testing.expectEqual(@as(i32, 0), try part2(example, std.testing.allocator));
}

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();

    // https://adventofcode.com/$Year/day/$Day
    const input = @embedFile("input");
    std.debug.print("Day $Day, ${Year}:\n", .{});
    std.debug.print("  Part 1: {}\n", .{try part1(input, allocator)});
    std.debug.print("  Part 2: {}\n", .{try part2(input, allocator)});
}
"@

    $zigFilepath = (Join-Path $PSScriptRoot "Src\$Year\day$Day.zig")

    if (Test-Path $zigFilepath)
    {
        Write-Warning "$Year\day$Day.zig already exists"
    }
    else
    {
        Set-Content -Path $zigFilepath -Value $zigText -Encoding utf8
    }

    return
}

$cppText = @"
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

$hppText = @"
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
    Set-Content -Path $cppFilepath -Value $cppText -Encoding utf8
    Set-Content -Path $hppFilepath -Value $hppText -Encoding utf8
    Add-Content -Path (Join-Path $PSScriptRoot 'CMakeLists.txt') -Value "add_aoc($Year $Day)" -Encoding utf8
}

$inputFile = (Join-Path $PSScriptRoot "inputs\$Year\day$Day.txt")
if (-not (Test-Path $inputFile))
{
    Set-Content -Path $inputFile -Value "" -NoNewline -Encoding utf8
}
