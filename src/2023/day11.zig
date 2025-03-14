const std = @import("std");
const utils = @import("utils");
const Point2d = utils.Point2d(i64);

pub fn main() !void {
    // https://adventofcode.com/2023/day/11
    utils.printTitle(2023, 11, "Cosmic Expansion");

    const m = utils.Monitor.init();
    defer m.deinit();

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();

    const input = utils.trimInput(@embedFile("input"));
    utils.printAnswer(1, try part1(input, allocator));
    utils.printAnswer(2, try part2(input, allocator));
}

fn distance(a: Point2d, b: Point2d) i64 {
    return a.manhatanDistance(b);
}

fn expand(galaxies: *std.ArrayList(Point2d), factor: i64, allocator: std.mem.Allocator) !void {
    var setX = std.AutoHashMap(i64, void).init(allocator);
    defer setX.deinit();
    var setY = std.AutoHashMap(i64, void).init(allocator);
    defer setY.deinit();

    var min = galaxies.items[0];
    var max = galaxies.items[0];

    for (galaxies.items) |g| {
        min.x = @min(min.x, g.x);
        min.y = @min(min.y, g.x);
        max.x = @max(max.x, g.x);
        max.y = @max(max.y, g.x);
        try setX.put(g.x, {});
        try setY.put(g.y, {});
    }

    var x = max.x;
    while (x > min.x) {
        x -= 1;
        if (setX.get(x) == null) {
            var i: usize = 0;
            while (i != galaxies.items.len) : (i += 1) {
                if (galaxies.items[i].x > x) {
                    galaxies.items[i].x += factor;
                }
            }
        }
    }

    var y = max.y;
    while (y > min.y) {
        y -= 1;
        if (setY.get(y) == null) {
            var i: usize = 0;
            while (i != galaxies.items.len) : (i += 1) {
                if (galaxies.items[i].y > y) {
                    galaxies.items[i].y += factor;
                }
            }
        }
    }
}

fn simulate(input: []const u8, expansionFactor: i64, allocator: std.mem.Allocator) !i64 {
    var galaxies = std.ArrayList(Point2d).init(allocator);
    defer galaxies.deinit();

    var it = std.mem.tokenizeAny(u8, input, "\r\n");
    var y: i64 = 0;
    while (it.next()) |line| {
        var x: i64 = 0;
        for (line) |c| {
            if (c == '#') {
                const p = Point2d{ .x = x, .y = y };
                try galaxies.append(p);
            }
            x += 1;
        }
        y += 1;
    }

    try expand(&galaxies, @max(expansionFactor - 1, 1), allocator);
    var total: i64 = 0;
    for (galaxies.items, 0..) |a, i| {
        for (galaxies.items, 0..) |b, j| {
            if (j <= i) continue;
            total += a.manhatanDistance(b);
        }
    }

    return total;
}

fn part1(input: []const u8, allocator: std.mem.Allocator) !i64 {
    return simulate(input, 1, allocator);
}

fn part2(input: []const u8, allocator: std.mem.Allocator) !i64 {
    return simulate(input, 1000000, allocator);
}

test "part 1" {
    const example =
        \\...#......
        \\.......#..
        \\#.........
        \\..........
        \\......#...
        \\.#........
        \\.........#
        \\..........
        \\.......#..
        \\#...#.....
    ;
    try std.testing.expectEqual(@as(i64, 374), try part1(example, std.testing.allocator));
}

test "part 2" {
    const example =
        \\...#......
        \\.......#..
        \\#.........
        \\..........
        \\......#...
        \\.#........
        \\.........#
        \\..........
        \\.......#..
        \\#...#.....
    ;
    try std.testing.expectEqual(@as(i64, 1030), try simulate(example, 10, std.testing.allocator));
    try std.testing.expectEqual(@as(i64, 8410), try simulate(example, 100, std.testing.allocator));
}
