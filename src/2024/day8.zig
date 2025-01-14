const std = @import("std");
const utils = @import("utils");
const Point2d = utils.Point2d(i32);

pub fn main() !void {
    // https://adventofcode.com/2024/day/8
    utils.printTitle(2024, 8, "Resonant Collinearity");

    const m = utils.Monitor.init();
    defer m.deinit();
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const ally = gpa.allocator();
    const input = comptime utils.trimInput(@embedFile("input"));

    const p1 = try part1(input, ally);
    utils.printAnswer(1, p1);
    std.debug.assert(276 == p1);

    const p2 = try part2(input, ally);
    utils.printAnswer(2, p2);
    std.debug.assert(991 == p2);
}

const Map = struct {
    frequencies: std.AutoHashMap(u8, std.ArrayList(Point2d)),
    width: i32,
    height: i32,

    pub fn parse(input: []const u8, ally: std.mem.Allocator) !Map {
        var frequencies = std.AutoHashMap(u8, std.ArrayList(Point2d)).init(ally);
        var width: i32 = 0;
        var y: i32 = 0;
        var it = std.mem.tokenizeAny(u8, input, "\r\n");
        while (it.next()) |line| : (y += 1) {
            for (0.., line) |x, c| {
                if (c != '.') {
                    const p = Point2d{ .x = @intCast(x), .y = y };

                    if (frequencies.getPtr(c)) |list| {
                        try list.append(p);
                    } else {
                        var list = std.ArrayList(Point2d).init(ally);
                        try list.append(p);
                        try frequencies.put(c, list);
                    }
                }

                width = @max(width, @as(i32, @intCast(x)) + 1);
            }
        }

        return .{
            .frequencies = frequencies,
            .width = width,
            .height = y,
        };
    }

    pub fn deinit(self: *@This()) void {
        var it = self.frequencies.valueIterator();
        while (it.next()) |l| {
            l.deinit();
        }
        self.frequencies.deinit();
    }

    pub fn contains(self: *const @This(), p: Point2d) bool {
        return p.x >= 0 and p.y >= 0 and p.x < self.width and p.y < self.height;
    }
};

fn part1(input: []const u8, ally: std.mem.Allocator) !i32 {
    var map = try Map.parse(input, ally);
    defer map.deinit();

    var antinodes = std.AutoHashMap(Point2d, void).init(ally);
    defer antinodes.deinit();

    var it = map.frequencies.valueIterator();
    while (it.next()) |frequency| {
        for (0..frequency.items.len) |i| {
            for (i + 1..frequency.items.len) |j| {
                const a = frequency.items[i];
                const b = frequency.items[j];
                const diff = b.subp(a);

                const c = a.subp(diff);
                if (map.contains(c)) {
                    try antinodes.put(c, {});
                }

                const d = b.addp(diff);
                if (map.contains(d)) {
                    try antinodes.put(d, {});
                }
            }
        }
    }

    return @intCast(antinodes.count());
}

fn part2(input: []const u8, ally: std.mem.Allocator) !i32 {
    var map = try Map.parse(input, ally);
    defer map.deinit();

    var antinodes = std.AutoHashMap(Point2d, void).init(ally);
    defer antinodes.deinit();

    var it = map.frequencies.valueIterator();
    while (it.next()) |frequency| {
        for (0..frequency.items.len) |i| {
            for (i + 1..frequency.items.len) |j| {
                var a = frequency.items[i];
                var b = frequency.items[j];
                const diff = b.subp(a);

                while (map.contains(a)) : (a = a.subp(diff)) {
                    try antinodes.put(a, {});
                }

                while (map.contains(b)) : (b = b.addp(diff)) {
                    try antinodes.put(b, {});
                }
            }
        }
    }

    return @intCast(antinodes.count());
}

test "part 1,2" {
    const example =
        \\............
        \\........0...
        \\.....0......
        \\.......0....
        \\....0.......
        \\......A.....
        \\............
        \\............
        \\........A...
        \\.........A..
        \\............
        \\............
    ;
    try std.testing.expectEqual(14, try part1(example, std.testing.allocator));
    try std.testing.expectEqual(34, try part2(example, std.testing.allocator));
}
