const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/2025/day/7
    utils.printTitle(2025, 7, "Laboratories");

    const m = utils.Monitor.init();
    defer m.deinit();
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const ally = gpa.allocator();
    const input = comptime utils.trimInput(@embedFile("input"));

    var grid = try Grid.parse(ally, input);
    defer grid.deinit();

    const p1 = try grid.part1();
    utils.printAnswer(1, p1);
    std.debug.assert(1675 == p1);

    const p2 = try grid.part2();
    utils.printAnswer(2, p2);
    std.debug.assert(187987920774390 == p2);
}

const Int = u64;
const P2d = utils.Point2d(i32);
const CacheKey = struct {
    pos: P2d,
    dir: P2d,
};

const Grid = struct {
    map: std.AutoHashMap(P2d, u8),
    width: u32 = 0,
    height: u32 = 0,
    start: P2d = P2d{},
    visited: std.AutoHashMap(P2d, void),
    cache: std.AutoHashMap(CacheKey, Int),
    ally: std.mem.Allocator,

    pub fn parse(ally: std.mem.Allocator, input: []const u8) !Grid {
        var result = Grid{
            .map = std.AutoHashMap(P2d, u8).init(ally),
            .visited = std.AutoHashMap(P2d, void).init(ally),
            .cache = std.AutoHashMap(CacheKey, Int).init(ally),
            .ally = ally,
        };

        var lineIt = std.mem.tokenizeAny(u8, input, "\r\n");
        while (lineIt.next()) |line| {
            result.width = @max(result.width, @as(u32, @intCast(line.len)));

            for (0.., line) |x, c| {
                const p = P2d{ .x = @intCast(x), .y = @intCast(result.height) };
                if (c == 'S') {
                    result.start = p;
                }
                try result.map.put(p, c);
            }

            result.height += 1;
        }

        return result;
    }

    pub fn deinit(self: *@This()) void {
        self.map.deinit();
        self.visited.deinit();
        self.cache.deinit();
    }

    fn countSplits(self: *@This(), pos: P2d) !Int {
        var p = pos.south();
        while (p.y < self.height) : (p = p.south()) {
            if (try self.visited.fetchPut(p, {}) != null) {
                break;
            }

            if (self.map.get(p)) |c| {
                if (c == '^') {
                    return 1 +
                        try self.countSplits(p.west()) +
                        try self.countSplits(p.east());
                }
            }
        }

        return 0;
    }

    fn countTimelines(self: *@This(), pos: P2d, dir: P2d) !Int {
        const key = CacheKey{ .pos = pos, .dir = dir };

        if (self.cache.get(key)) |v| {
            return v;
        }

        var p = pos.addp(dir).south();

        while (p.y < self.height) : (p = p.south()) {
            if (self.map.get(p)) |c| {
                if (c == '^') {
                    const total = try self.countTimelines(p, P2d.WEST) +
                        try self.countTimelines(p, P2d.EAST);
                    try self.cache.put(key, total);
                    return total;
                }
            }
        }

        return 1;
    }

    pub fn part1(self: *@This()) !Int {
        return try self.countSplits(self.start);
    }

    pub fn part2(self: *@This()) !Int {
        return try self.countTimelines(self.start, P2d.WEST) +
            try self.countTimelines(self.start, P2d.EAST);
    }
};

test "parts 1,2" {
    const ally = std.testing.allocator;
    const example =
        \\.......S.......
        \\...............
        \\.......^.......
        \\...............
        \\......^.^......
        \\...............
        \\.....^.^.^.....
        \\...............
        \\....^.^...^....
        \\...............
        \\...^.^...^.^...
        \\...............
        \\..^...^.....^..
        \\...............
        \\.^.^.^.^.^...^.
        \\...............
    ;
    var grid = try Grid.parse(ally, example);
    defer grid.deinit();

    try std.testing.expectEqual(21, try grid.part1());
    try std.testing.expectEqual(40, try grid.part2());
}
