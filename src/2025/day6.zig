const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/2025/day/6
    utils.printTitle(2025, 6, "Trash Compactor");

    const m = utils.Monitor.init();
    defer m.deinit();
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const ally = gpa.allocator();
    const input = comptime utils.trimInput(@embedFile("input"));

    const p1 = try part1(ally, input);
    utils.printAnswer(1, p1);
    std.debug.assert(7098065460541 == p1);

    const p2 = try part2(ally, input);
    utils.printAnswer(2, p2);
    std.debug.assert(13807151830618 == p2);
}

const Int = u64;

fn part1(ally: std.mem.Allocator, input: []const u8) !Int {
    var numbers = std.ArrayList(Int).empty;
    defer numbers.deinit(ally);
    var operators = std.ArrayList(u8).empty;
    defer operators.deinit(ally);

    var list = std.ArrayList([]Int).empty;
    defer {
        for (list.items) |nums| ally.free(nums);
        list.deinit(ally);
    }

    var lineIt = std.mem.tokenizeAny(u8, input, "\r\n");
    while (lineIt.next()) |line| {
        var tokenIt = std.mem.tokenizeScalar(u8, line, ' ');
        numbers.clearRetainingCapacity();

        while (tokenIt.next()) |token| {
            if (token[0] == '*' or token[0] == '+') {
                try operators.append(ally, token[0]);
                continue;
            }

            const num = try std.fmt.parseInt(Int, token, 10);
            try numbers.append(ally, num);
        }

        if (numbers.items.len == 0) continue;
        try list.append(ally, try numbers.toOwnedSlice(ally));
    }

    var result: Int = 0;
    for (0.., operators.items) |i, op| {
        result += switch (op) {
            '+' => add: {
                var value: Int = 0;
                for (list.items) |nums| value += nums[i];
                break :add value;
            },
            '*' => mul: {
                var value: Int = 1;
                for (list.items) |nums| value *= nums[i];
                break :mul value;
            },
            else => unreachable,
        };
    }

    return result;
}

const Operation = enum {
    none,
    add,
    mul,
};

fn part2(ally: std.mem.Allocator, input: []const u8) !Int {
    var lines = std.ArrayList([]const u8).empty;
    defer lines.deinit(ally);

    var maxLineLen: usize = 0;
    var lineIt = std.mem.tokenizeAny(u8, input, "\r\n");
    while (lineIt.next()) |line| {
        maxLineLen = @max(maxLineLen, line.len);
        try lines.append(ally, line);
    }

    var numbers = std.ArrayList(Int).empty;
    defer numbers.deinit(ally);
    var total: Int = 0;

    for (0..maxLineLen) |i| {
        const idx = maxLineLen - i - 1;
        var num: ?Int = null;
        var op: Operation = .none;

        for (lines.items) |line| {
            const c = if (idx < line.len) line[idx] else ' ';

            switch (c) {
                ' ' => continue,
                '+' => op = .add,
                '*' => op = .mul,
                else => {
                    const n = std.fmt.charToDigit(c, 10) catch unreachable;
                    num = (num orelse 0) * 10 + n;
                },
            }
        }

        if (num) |n| {
            try numbers.append(ally, n);
        }

        switch (op) {
            .add => {
                for (numbers.items) |n| {
                    total += n;
                }
                numbers.clearRetainingCapacity();
            },
            .mul => {
                var value: Int = 1;
                for (numbers.items) |n| {
                    value *= n;
                }
                total += value;
                numbers.clearRetainingCapacity();
            },
            .none => {},
        }
    }

    return total;
}

test "parts 1,2" {
    const ally = std.testing.allocator;
    const example =
        \\123 328  51 64
        \\ 45 64  387 23
        \\  6 98  215 314
        \\*   +   *   +
    ;
    try std.testing.expectEqual(4277556, try part1(ally, example));
    try std.testing.expectEqual(3263827, try part2(ally, example));
}
