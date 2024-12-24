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

const Int = i32;
const String = []const u8;
const Point2d = utils.Point2d(Int);
const Keypad = struct {
    keys: []const String,
    start: Point2d,
    w: usize,
    h: usize,
};

// Numeric keypad:
// +---+---+---+
// | 7 | 8 | 9 |
// +---+---+---+
// | 4 | 5 | 6 |
// +---+---+---+
// | 1 | 2 | 3 |
// +---+---+---+
//     | 0 | A |
//     +---+---+
const NUMERIC_KEY_PAD = Keypad{
    .keys = &.{
        "789",
        "456",
        "123",
        " 0A",
    },
    .start = .{ .x = 2, .y = 3 },
    .w = 3,
    .h = 4,
};

// Directional keypad:
//     +---+---+
//     | ^ | A |
// +---+---+---+
// | < | v | > |
// +---+---+---+
const DIRECTIONAL_KEY_PAD = Keypad{
    .keys = &.{
        " ^A",
        "<v>",
    },
    .start = .{ .x = 2, .y = 0 },
    .w = 3,
    .h = 2,
};

const Direction = enum {
    A,
    Up,
    Down,
    Left,
    Right,
};

fn part1(ally: std.mem.Allocator, input: []const u8) !Int {
    var combos = std.ArrayList([]const u8).init(ally);
    defer combos.deinit();

    var lineIt = std.mem.tokenizeAny(u8, input, "\r\n");
    while (lineIt.next()) |line| {
        try combos.append(line);
    }

    std.debug.print("{any}, {c}\n", .{ NUMERIC_KEY_PAD, NUMERIC_KEY_PAD.keys[1][0] });

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
        \\029A
        \\980A
        \\179A
        \\456A
        \\379A
    ;
    try std.testing.expectEqual(0, try part1(ally, example));
    try std.testing.expectEqual(0, try part2(ally, example));
}
