const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/2024/day/2
    utils.printTitle(2024, 2, "Red-Nosed Reports");

    const m = utils.Monitor.init();
    defer m.deinit();
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const ally = gpa.allocator();
    const input = comptime utils.trimInput(@embedFile("input"));

    const p1 = try part1(input, ally);
    utils.printAnswer(1, p1);
    std.debug.assert(564 == p1);

    const p2 = try part2(input, ally);
    utils.printAnswer(2, p2);
    std.debug.assert(604 == p2);
}

const Direction = enum {
    unknown,
    increasing,
    decreasing,
};

fn is_safe(report: []const i32, skip: ?usize) bool {
    var dir = Direction.unknown;
    var last: ?i32 = null;

    for (0.., report) |i, n| {
        if (skip == i) {
            continue;
        }

        if (last == null) {
            last = n;
            continue;
        }

        const diff = last.? - n;

        switch (dir) {
            .unknown => {
                dir = if (diff < 0) .decreasing else .increasing;
            },
            .increasing => {
                if (diff < 0) return false;
            },
            .decreasing => {
                if (diff > 0) return false;
            },
        }

        const abs_diff = @abs(diff);

        if (abs_diff < 1 or abs_diff > 3) {
            return false;
        }

        last = n;
    }

    return true;
}

fn parse_report(line: []const u8, ally: std.mem.Allocator) !std.ArrayList(i32) {
    var report = std.ArrayList(i32).init(ally);
    var line_it = std.mem.tokenizeScalar(u8, line, ' ');

    while (line_it.next()) |value| {
        const num = try std.fmt.parseInt(i32, value, 10);
        try report.append(num);
    }

    return report;
}

fn part1(input: []const u8, ally: std.mem.Allocator) !i32 {
    var count: i32 = 0;

    var it = std.mem.tokenizeAny(u8, input, "\r\n");
    while (it.next()) |line| {
        var report = try parse_report(line, ally);
        defer report.deinit();

        if (is_safe(report.items, null)) {
            count += 1;
        }
    }

    return count;
}

test "part 1" {
    const example =
        \\7 6 4 2 1
        \\1 2 7 8 9
        \\9 7 6 2 1
        \\1 3 2 4 5
        \\8 6 4 4 1
        \\1 3 6 7 9
    ;
    try std.testing.expectEqual(2, try part1(example, std.testing.allocator));
}

fn is_safe_with_damper(report: []i32) bool {
    if (is_safe(report, null))
        return true;

    for (0.., report) |i, _| {
        if (is_safe(report, i)) {
            return true;
        }
    }

    return false;
}

fn part2(input: []const u8, ally: std.mem.Allocator) !i32 {
    var count: i32 = 0;

    var it = std.mem.tokenizeAny(u8, input, "\r\n");
    while (it.next()) |line| {
        var report = try parse_report(line, ally);
        defer report.deinit();

        if (is_safe_with_damper(report.items)) {
            count += 1;
        }
    }

    return count;
}

test "part 2" {
    const example =
        \\7 6 4 2 1
        \\1 2 7 8 9
        \\9 7 6 2 1
        \\1 3 2 4 5
        \\8 6 4 4 1
        \\1 3 6 7 9
    ;
    try std.testing.expectEqual(4, try part2(example, std.testing.allocator));
}
