const std = @import("std");

const History = struct {
    lists: std.ArrayList(std.ArrayList(i32)),

    pub fn init(numbersText: []const u8, allocator: std.mem.Allocator) !History {
        var result = History{ .lists = std.ArrayList(std.ArrayList(i32)).init(allocator) };
        try result.lists.append(std.ArrayList(i32).init(allocator));
        var itNumber = std.mem.tokenizeScalar(u8, numbersText, ' ');
        while (itNumber.next()) |numText| {
            try result.lists.items[result.lists.items.len - 1].append(try std.fmt.parseInt(i32, numText, 10));
        }

        return result;
    }

    pub fn reduce(self: *@This(), allocator: std.mem.Allocator) !bool {
        var more: bool = false;
        var newList = std.ArrayList(i32).init(allocator);
        var last: ?i32 = null;
        for (self.lists.getLast().items) |num| {
            if (last) |l| {
                const diff = num - l;
                if (diff != 0) {
                    more = true;
                }
                try newList.append(diff);
            }
            last = num;
        }

        try self.lists.append(newList);
        return more;
    }

    pub fn nextValue(self: *const @This()) i32 {
        var val: ?i32 = null;

        var i: usize = self.lists.items.len - 1;
        while (i > 0) {
            i -= 1;
            val = (val orelse 0) + self.lists.items[i].getLast();
        }

        return val.?;
    }

    pub fn prevValue(self: *const @This()) i32 {
        var val: ?i32 = null;

        var i: usize = self.lists.items.len - 1;
        while (i > 0) {
            i -= 1;
            val = self.lists.items[i].items[0] - (val orelse 0);
        }

        return val.?;
    }

    pub fn deinit(self: *const @This()) void {
        for (self.lists.items) |list| {
            list.deinit();
        }
        self.lists.deinit();
    }
};

fn part1(input: []const u8, allocator: std.mem.Allocator) !i32 {
    var lists = std.ArrayList(History).init(allocator);
    defer lists.deinit();

    var it = std.mem.tokenizeAny(u8, input, "\r\n");
    while (it.next()) |line| {
        var hist = try History.init(line, allocator);
        while (try hist.reduce(allocator)) {}
        try lists.append(hist);
    }

    var sum: i32 = 0;
    for (lists.items) |hist| {
        sum += hist.nextValue();
        hist.deinit();
    }

    return sum;
}

test "part 1" {
    const example =
        \\0 3 6 9 12 15
        \\1 3 6 10 15 21
        \\10 13 16 21 30 45
    ;
    try std.testing.expectEqual(@as(i32, 114), try part1(example, std.testing.allocator));
}

fn part2(input: []const u8, allocator: std.mem.Allocator) !i32 {
    var lists = std.ArrayList(History).init(allocator);
    defer lists.deinit();

    var it = std.mem.tokenizeAny(u8, input, "\r\n");
    while (it.next()) |line| {
        var hist = try History.init(line, allocator);
        while (try hist.reduce(allocator)) {}
        try lists.append(hist);
    }

    var sum: i32 = 0;
    for (lists.items) |hist| {
        sum += hist.prevValue();
        hist.deinit();
    }

    return sum;
}

test "part 2" {
    const example =
        \\0 3 6 9 12 15
        \\1 3 6 10 15 21
        \\10 13 16 21 30 45
    ;
    try std.testing.expectEqual(@as(i32, 2), try part2(example, std.testing.allocator));
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
