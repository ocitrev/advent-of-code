const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/2024/day/1
    std.debug.print("Day 1, 2024: Historian Hysteria\n", .{});

    const m = utils.Monitor.init();
    defer m.deinit();
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();
    const input = comptime utils.trim_input(@embedFile("input"));

    const p1 = try part1(input, allocator);
    std.debug.print("  Part 1: {}\n", .{p1});
    std.debug.assert(2769675 == p1);

    const p2 = try part2(input, allocator);
    std.debug.print("  Part 2: {}\n", .{p2});
    std.debug.assert(24643097 == p2);
}

const Lists = struct {
    a: std.ArrayList(i32),
    b: std.ArrayList(i32),

    pub fn init(ally: std.mem.Allocator) Lists {
        return Lists{
            .a = std.ArrayList(i32).init(ally),
            .b = std.ArrayList(i32).init(ally),
        };
    }

    pub fn parse(self: *@This(), input: []const u8) !void {
        var it = std.mem.tokenizeAny(u8, input, "\r\n");
        while (it.next()) |line| {
            var line_it = std.mem.tokenizeScalar(u8, line, ' ');
            const a = line_it.next().?;
            const b = line_it.next().?;
            try self.a.append(try std.fmt.parseInt(i32, a, 10));
            try self.b.append(try std.fmt.parseInt(i32, b, 10));
        }
    }

    pub fn deinit(self: *@This()) void {
        self.a.deinit();
        self.b.deinit();
    }
};

fn part1(input: []const u8, ally: std.mem.Allocator) !i32 {
    var lists = Lists.init(ally);
    defer lists.deinit();
    try lists.parse(input);

    std.mem.sort(i32, lists.a.items, {}, std.sort.asc(i32));
    std.mem.sort(i32, lists.b.items, {}, std.sort.asc(i32));

    var sum: i32 = 0;
    for (lists.a.items, lists.b.items) |a, b| {
        sum += @max(a, b) - @min(a, b);
    }

    return sum;
}

test "part 1" {
    const example =
        \\3   4
        \\4   3
        \\2   5
        \\1   3
        \\3   9
        \\3   3
    ;
    try std.testing.expectEqual(11, try part1(example, std.testing.allocator));
}

fn part2(input: []const u8, ally: std.mem.Allocator) !i32 {
    var lists = Lists.init(ally);
    defer lists.deinit();
    try lists.parse(input);

    var sum: i32 = 0;
    for (lists.a.items) |a| {
        var count: i32 = 0;
        for (lists.b.items) |b| {
            if (a == b) {
                count += 1;
            }
        }

        sum += a * count;
    }

    return sum;
}

test "part 2" {
    const example =
        \\3   4
        \\4   3
        \\2   5
        \\1   3
        \\3   9
        \\3   3
    ;
    try std.testing.expectEqual(31, try part2(example, std.testing.allocator));
}
