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

const Column = struct {
    start: usize,
    len: usize,
    op: u8,
};

fn part2(ally: std.mem.Allocator, input: []const u8) !Int {
    var columns = std.ArrayList(Column).empty;
    defer columns.deinit(ally);

    var lines = std.ArrayList([]const u8).empty;
    defer {
        for (lines.items) |line| ally.free(line);
        lines.deinit(ally);
    }

    var maxLineLen: usize = 0;
    var lineIt = std.mem.tokenizeAny(u8, input, "\r\n");
    while (lineIt.next()) |line| {
        maxLineLen = @max(maxLineLen, line.len);
    }

    lineIt = std.mem.tokenizeAny(u8, input, "\r\n");
    while (lineIt.next()) |line| {
        if (line[0] == '*' or line[0] == '+') {
            var col = Column{ .start = 0, .len = 0, .op = line[0] };

            for (1.., line[1..]) |i, c| {
                if (c == '*' or c == '+') {
                    col.len = i - col.start - 1;
                    try columns.append(ally, col);
                    col.start = i;
                    col.op = c;
                }
            }

            col.len = maxLineLen - col.start;
            try columns.append(ally, col);
        } else {
            const paddedLine = try ally.alloc(u8, maxLineLen);
            for (paddedLine) |*b| b.* = ' ';
            std.mem.copyForwards(u8, paddedLine, line);
            try lines.append(ally, paddedLine);
        }
    }

    var total: Int = 0;
    for (columns.items) |col| {
        var value: Int = if (col.op == '+') 0 else 1;

        for (0..col.len) |i| {
            var num: Int = 0;
            for (lines.items) |line| {
                const c = line[col.start + i];
                if (c == ' ') continue;
                num = num * 10 + (std.fmt.charToDigit(c, 10) catch unreachable);
            }

            switch (col.op) {
                '+' => value += num,
                '*' => value *= num,
                else => unreachable,
            }
        }

        total += value;
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
