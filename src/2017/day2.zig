const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();

    // https://adventofcode.com/2017/day/2
    const input = comptime utils.trim_input(@embedFile("input"));
    std.debug.print("Day 2, 2017: Corruption Checksum\n", .{});

    @setEvalBranchQuota(100_000);
    const p1 = comptime part1(input);
    std.debug.print("  Part 1: {}\n", .{p1});
    std.debug.assert(45972 == p1);

    const p2 = try part2(input, allocator);
    std.debug.print("  Part 2: {}\n", .{p2});
    std.debug.assert(326 == p2);
}

fn checksum(line: []const u8) i32 {
    var min: ?i32 = null;
    var max: ?i32 = null;
    var row = std.mem.tokenizeAny(u8, line, " \t");
    while (row.next()) |cell| {
        const num = std.fmt.parseInt(i32, cell, 10) catch unreachable;
        min = @min(min orelse num, num);
        max = @max(max orelse num, num);
    }

    return max.? - min.?;
}

fn part1(input: []const u8) i32 {
    var sum: i32 = 0;
    var lines = std.mem.tokenizeAny(u8, input, "\r\n");
    while (lines.next()) |line| {
        sum += checksum(line);
    }

    return sum;
}

test "part 1" {
    const example =
        \\5 1 9 5
        \\7 5 3
        \\2 4 6 8
    ;
    try std.testing.expectEqual(18, comptime part1(example));
}

fn checksum_2(line: []const u8, allocator: std.mem.Allocator) !i32 {
    var numbers = std.ArrayList(i32).init(allocator);
    defer numbers.deinit();

    var row = std.mem.tokenizeAny(u8, line, " \t");
    while (row.next()) |cell| {
        const num = try std.fmt.parseInt(i32, cell, 10);
        try numbers.append(num);
    }

    for (0.., numbers.items) |i, a| {
        for (numbers.items[i + 1 ..]) |b| {
            if (a == b) continue;
            if (@rem(a, b) == 0) return @divExact(a, b);
            if (@rem(b, a) == 0) return @divExact(b, a);
        }
    }

    unreachable;
}

fn part2(input: []const u8, allocator: std.mem.Allocator) !i32 {
    var sum: i32 = 0;
    var lines = std.mem.tokenizeAny(u8, input, "\r\n");
    while (lines.next()) |line| {
        sum += try checksum_2(line, allocator);
    }

    return sum;
}

test "part 2" {
    const example =
        \\5 9 2 8
        \\9 4 7 3
        \\3 8 6 5
    ;
    try std.testing.expectEqual(4, try checksum_2("5 9 2 8", std.testing.allocator));
    try std.testing.expectEqual(3, try checksum_2("9 4 7 3", std.testing.allocator));
    try std.testing.expectEqual(2, try checksum_2("3 8 6 5", std.testing.allocator));
    try std.testing.expectEqual(9, try part2(example, std.testing.allocator));
}
