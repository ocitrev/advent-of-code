const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/2024/day/21
    std.debug.print("Day 21, 2024: Keypad Conundrum\n", .{});

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
