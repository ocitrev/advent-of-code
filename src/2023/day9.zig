const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/2023/day/9
    utils.printTitle(2023, 9, "Mirage Maintenance");

    const m = utils.Monitor.init();
    defer m.deinit();

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();

    const input = comptime utils.trimInput(@embedFile("input"));
    utils.printAnswer(1, try part1(input, allocator));
    utils.printAnswer(2, try part2(input, allocator));
}

const History = struct {
    lists: std.ArrayList(std.ArrayList(i32)),
    allocator: std.mem.Allocator,

    pub fn init(numbersText: []const u8, allocator: std.mem.Allocator) !History {
        var result = History{
            .lists = std.ArrayList(std.ArrayList(i32)).empty,
            .allocator = allocator,
        };
        try result.lists.append(allocator, std.ArrayList(i32).empty);
        var itNumber = std.mem.tokenizeScalar(u8, numbersText, ' ');
        while (itNumber.next()) |numText| {
            try result.lists.items[result.lists.items.len - 1].append(allocator, try std.fmt.parseInt(i32, numText, 10));
        }

        return result;
    }

    pub fn reduce(self: *@This()) !bool {
        var more: bool = false;
        var newList = std.ArrayList(i32).empty;
        var last: ?i32 = null;
        for (self.lists.getLast().items) |num| {
            if (last) |l| {
                const diff = num - l;
                if (diff != 0) {
                    more = true;
                }
                try newList.append(self.allocator, diff);
            }
            last = num;
        }

        try self.lists.append(self.allocator, newList);
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

    pub fn deinit(self: *@This()) void {
        for (self.lists.items) |*list| {
            list.deinit(self.allocator);
        }
        self.lists.deinit(self.allocator);
    }
};

fn part1(input: []const u8, allocator: std.mem.Allocator) !i32 {
    var lists = std.ArrayList(History).empty;
    defer lists.deinit(allocator);

    var it = std.mem.tokenizeAny(u8, input, "\r\n");
    while (it.next()) |line| {
        var hist = try History.init(line, allocator);
        while (try hist.reduce()) {}
        try lists.append(allocator, hist);
    }

    var sum: i32 = 0;
    for (lists.items) |*hist| {
        sum += hist.nextValue();
        hist.deinit();
    }

    return sum;
}

fn part2(input: []const u8, allocator: std.mem.Allocator) !i32 {
    var lists = std.ArrayList(History).empty;
    defer lists.deinit(allocator);

    var it = std.mem.tokenizeAny(u8, input, "\r\n");
    while (it.next()) |line| {
        var hist = try History.init(line, allocator);
        while (try hist.reduce()) {}
        try lists.append(allocator, hist);
    }

    var sum: i32 = 0;
    for (lists.items) |*hist| {
        sum += hist.prevValue();
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

test "part 2" {
    const example =
        \\0 3 6 9 12 15
        \\1 3 6 10 15 21
        \\10 13 16 21 30 45
    ;
    try std.testing.expectEqual(@as(i32, 2), try part2(example, std.testing.allocator));
}
