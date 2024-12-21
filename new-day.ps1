#Requires -Version 7

[CmdletBinding()]
param(
    [int]$Year = [datetime]::Now.Year,
    [int]$Day = [datetime]::Now.AddHours(+1).Day,
    [switch]$Zig,
    [switch]$Rust
)

function get-year-folder
{
    $folder = Join-Path $PSScriptRoot Src $Year

    if (-not (Test-Path $folder))
    {
        $null = mkdir $folder
    }

    return $folder
}

if ($Zig)
{
    $zigText = @"
const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/$Year/day/$Day
    std.debug.print("Day $Day, ${Year}: \n", .{});

    const m = utils.Monitor.init();
    defer m.deinit();
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const ally = gpa.allocator();
    const input = comptime utils.trim_input(@embedFile("input"));

    const p1 = try part1(ally, input);
    utils.printAnswer(1, p1);
    // std.debug.assert(0 == p1);

    const p2 = try part2(ally, input);
    utils.printAnswer(2, p2);
    // std.debug.assert(0 == p2);
}

fn part1(ally: std.mem.Allocator, input: []const u8) !i32 {
    _ = ally;
    
    var lineIt = std.mem.tokenizeAny(u8, input, "\r\n");
    while (lineIt.next()) |line| {
        _ = line;
    }

    return 0;
}

fn part2(ally: std.mem.Allocator, input: []const u8) !i32 {
    _ = ally;
    
    var lineIt = std.mem.tokenizeAny(u8, input, "\r\n");
    while (lineIt.next()) |line| {
        _ = line;
    }

    return 0;
}

test "parts 1,2" {
    const ally = std.testing.allocator;
    const example =
        \\
    ;
    try std.testing.expectEqual(0, try part1(ally, example));
    try std.testing.expectEqual(0, try part2(ally, example));
}    
"@

    $folder = get-year-folder
    $zigFilepath = (Join-Path $folder "day$Day.zig")

    if (Test-Path $zigFilepath)
    {
        Write-Warning "$Year\day$Day.zig already exists"
    }
    else
    {
        Set-Content -Path $zigFilepath -Value $zigText -Encoding utf8
    }

    if ($env:VSCODE_INJECTION -eq '1')
    {
        code $zigFilepath
    }
}
elseif ($Rust)
{
    $rustTemplate = @"
#[path = "../utils.rs"]
mod utils;

fn main() {
    // https://adventofcode.com/$Year/day/$Day
    println!("Day $Day, ${Year}: ");
    let _m = utils::Monitor::start();

    debug_assert_eq!(0, part1(get_example()));
    debug_assert_eq!(0, part2(get_example()));

    let p1 = part1(get_input());
    println!("  Part 1: {}", p1);
    // assert_eq!(0, p1);

    let p2 = part2(get_input());
    println!("  Part 2: {}", p2);
    // assert_eq!(0, p2);
}

fn part1(input: &'static str) -> u64 {
    0
}

fn part2(input: &'static str) -> u64 {
    0
}

fn get_input() -> &'static str {
    include_str!("../../inputs/$Year/day$Day.txt").trim_end()
}

fn get_example() -> &'static str {
    ""
}
"@

    $folder = get-year-folder
    $rustFilepath = (Join-Path $folder "day$Day.rs")

    if (Test-Path $rustFilepath)
    {
        Write-Warning "$Year\day$Day.zig already exists"
    }
    else
    {
        Set-Content -Path $rustFilepath -Value $rustTemplate -Encoding utf8
    }

    if ($env:VSCODE_INJECTION -eq '1')
    {
        code $rustFilepath
    }
}
else
{
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

    $folder = get-year-folder
    $cppFilepath = (Join-Path $folder "day$Day.cpp")
    $hppFilepath = (Join-Path $folder "day$Day.hpp")

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

    if ($env:VSCODE_INJECTION -eq '1')
    {
        code $cppFilepath
    }
}

$inputFile = (Join-Path $PSScriptRoot "inputs\$Year\day$Day.txt")

if (-not (Test-Path $inputFile))
{
    Set-Content -Path $inputFile -Value "" -NoNewline -Encoding utf8
}
