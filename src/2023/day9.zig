const std = @import("std");

fn part1(input: []const u8, allocator: std.mem.Allocator) !u32 {
    _ = allocator;
    _ = input;
    return 0;
}

test "part 1" {
    const example =
        \\
    ;
    _ = example;
}

fn part2(input: []const u8, allocator: std.mem.Allocator) !u64 {
    _ = allocator;
    _ = input;
    return 0;
}

test "part 2" {
    const example =
        \\
    ;
    _ = example;
}

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();

    // https://adventofcode.com/2023/day/9
    const input = @embedFile("input");
    std.debug.print("Day 9, 2023: Mirage Maintenance\n", .{});
    std.debug.print("  Part 1: {}\n", .{try part1(input, allocator)});
    std.debug.print("  Part 2: {}\n", .{try part2(input, allocator)});
}
