#!/usr/bin/env pwsh
#requires -version 7

[CmdletBinding(defaultparametersetname='zig')]

param(
    [int]$Year = [datetime]::Now.Year,
    [int]$Day = [datetime]::Now.AddHours(+1).Day,

    [parameter(parametersetname='cpp')]
    [switch]$Cpp,

    [parameter(parametersetname='zig')]
    [switch]$Zig,

    [parameter(parametersetname='rust')]
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

function create-zig
{
    $zigText = @"
const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/$Year/day/$Day
    utils.printTitle($Year, $Day, "");

    const m = utils.Monitor.init();
    defer m.deinit();
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const ally = gpa.allocator();
    const input = comptime utils.trimInput(@embedFile("input"));

    const p1 = try part1(ally, input);
    utils.printAnswer(1, p1);
    // std.debug.assert(0 == p1);

    const p2 = try part2(ally, input);
    utils.printAnswer(2, p2);
    // std.debug.assert(0 == p2);
}

const Int = u64;

fn part1(ally: std.mem.Allocator, input: []const u8) !Int {
    _ = ally;

    var lineIt = std.mem.tokenizeAny(u8, input, "\r\n");
    while (lineIt.next()) |line| {
        _ = line;
    }

    return 0;
}

fn part2(ally: std.mem.Allocator, input: []const u8) !Int {
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
        return
    }
    else
    {
        Set-Content -Path $zigFilepath -Value $zigText -Encoding utf8
    }

    if ($env:VSCODE_INJECTION -eq '1')
    {
        code $zigFilepath
    }

    # add this day to build.zig
    $buildDotZig = Join-Path $PSScriptRoot 'build.zig'
    (get-content $buildDotZig) | % -begin {
        $inBlock = $false
    } -process {
        $line = $_
        if ($inBlock) {
            if ($line -match '^(\s+)};') {
                write-output "$($matches[1])    .{ .year = $Year, .day = $Day },"
                $inBlock = $false
            }
        } elseif ($line -match 'const puzzles = \[_\]Aoc{') {
            $inBlock = $true
        }

        write-output $line
    } | set-content $buildDotZig -Encoding utf8
}

function create-rust
{
    $rustTemplate = @"
#[path = "../utils.rs"]
mod utils;

fn main() {
    // https://adventofcode.com/$Year/day/$Day
    println!("Day $Day, ${Year}: ");
    let _m = utils::Monitor::start();

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

#[cfg(test)]
mod tests {
    fn get_example() -> &'static str {
        indoc::indoc! {"
        "}.trim_end()
    }

    #[test]
    fn part1() {
        assert_eq!(0, super::part1(get_example()));
    }

    #[test]
    fn part1() {
        assert_eq!(0, super::part2(get_example()));
    }
}
"@

    $folder = get-year-folder
    $rustFilepath = (Join-Path $folder "day$Day.rs")

    if (Test-Path $rustFilepath)
    {
        Write-Warning "$Year\day$Day.rs already exists"
        return
    }
    else
    {
        Set-Content -Path $rustFilepath -Value $rustTemplate -Encoding utf8
    }

    $binName = "name = `"$Year-$Day`""
    # add to cargo.toml
    $cargoToml = Join-Path $PSScriptRoot 'Cargo.toml'
    (get-content $cargoToml) | % -begin {
        $inBlock = $false
        $found = $false
    } -process {
        $line = $_

        if ($line -match [regex]::escape($binName)) {
            write-warning "$Year-$Day binary already exists in Cargo.toml"
            $found = $true
        }

        if (-not $found -and $line -match '^# Insert new bin here') {
            write-output '[[bin]]'
            write-output $binName
            write-output "path = `"src/$Year/day$Day.rs`""
            write-output ""
        }

        write-output $line

    } | Set-Content -Path $cargoToml -Encoding utf8

    if ($env:VSCODE_INJECTION -eq '1')
    {
        code $rustFilepath
    }
}

function create-cpp
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
        return
    }
    elseif (Test-Path $hppFilepath)
    {
        Write-Warning "$Year\day$Day.hpp already exists"
        return
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

switch ($pscmdlet.ParameterSetName)
{
    'zig' { create-zig }
    'rust' { create-rust }
    'cpp' { create-cpp }
    default { throw "Unknown parameter set: $($pscmdlet.ParameterSetName)" }
}

$inputFile = (Join-Path $PSScriptRoot "inputs\$Year\day$Day.txt")

if (-not (Test-Path $inputFile))
{
    Set-Content -Path $inputFile -Value "" -NoNewline -Encoding utf8
}
