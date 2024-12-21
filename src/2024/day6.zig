const std = @import("std");
const utils = @import("utils");
const Point2d = utils.Point2d(i32);

pub fn main() !void {
    // https://adventofcode.com/2024/day/6
    std.debug.print("Day 6, 2024: Guard Gallivant\n", .{});

    const m = utils.Monitor.init();
    defer m.deinit();
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const ally = gpa.allocator();
    const input = comptime utils.trim_input(@embedFile("input"));

    var floor = try Floor.init(input, ally);
    defer floor.deinit();

    const p1 = try floor.part1();
    std.debug.print("  Part 1: {}\n", .{p1});
    std.debug.assert(5199 == p1);

    const p2 = try floor.part2();
    std.debug.print("  Part 2: {}\n", .{p2});
    std.debug.assert(1915 == p2);
}

const Floor = struct {
    ally: std.mem.Allocator,
    map: std.AutoHashMap(Point2d, u8),
    visited: std.AutoHashMap([2]Point2d, void),
    guard: Point2d,

    pub fn init(input: []const u8, ally: std.mem.Allocator) !Floor {
        var map = std.AutoHashMap(Point2d, u8).init(ally);
        var guard: Point2d = undefined;

        var line_it = std.mem.tokenizeAny(u8, input, "\r\n");
        var y: i32 = 0;
        while (line_it.next()) |line| : (y += 1) {
            for (0.., line) |x, c| {
                const p = Point2d{ .x = @intCast(x), .y = y };
                if (c == '^') {
                    guard = p;
                }
                try map.put(p, c);
            }
        }

        return .{
            .ally = ally,
            .map = map,
            .visited = std.AutoHashMap([2]Point2d, void).init(ally),
            .guard = guard,
        };
    }

    pub fn deinit(self: *@This()) void {
        self.map.deinit();
        self.visited.deinit();
    }

    fn run(self: *@This()) !i32 {
        var g = self.guard;
        var dir = Point2d.NORTH;
        var visited = std.AutoHashMap(Point2d, void).init(self.ally);
        defer visited.deinit();

        while (true) {
            try visited.put(g, {});
            const next = g.addp(dir);

            if (self.map.get(next)) |cell| {
                if (cell == '#') {
                    dir = dir.rotate90Right();
                } else {
                    g = next;
                }

                continue;
            }

            return @intCast(visited.count());
        }
    }

    fn hasLoop(self: *@This(), fromG: Point2d, fromD: Point2d, obstacle: Point2d) !bool {
        self.visited.clearRetainingCapacity();
        var g = fromG;
        var dir = fromD;

        while (true) {
            const next = g.addp(dir);
            if (self.map.get(next)) |c| {
                if (c == '#' or obstacle.eql(next)) {
                    dir = dir.rotate90Right();
                    if (try self.visited.fetchPut(.{ g, dir }, {}) != null) {
                        return true;
                    }
                } else {
                    g = next;
                }
            } else {
                return false;
            }
        }
    }

    fn runWithObstacle(self: *@This()) !i32 {
        var done = std.AutoHashMap(Point2d, void).init(self.ally);
        defer done.deinit();

        var g = self.guard;
        var dir = Point2d.NORTH;
        var result: i32 = 0;

        while (true) {
            const next = g.addp(dir);
            if (self.map.get(next)) |c| {
                if (c == '#') {
                    dir = dir.rotate90Right();
                } else {
                    if (try done.fetchPut(next, {}) == null and try self.hasLoop(g, dir, next)) {
                        result += 1;
                    }

                    g = next;
                }
            } else {
                return result;
            }
        }
    }

    pub fn part1(self: *@This()) !i32 {
        return self.run();
    }

    pub fn part2(self: *@This()) !i32 {
        return self.runWithObstacle();
    }
};

test "part 1,2" {
    const example =
        \\....#.....
        \\.........#
        \\..........
        \\..#.......
        \\.......#..
        \\..........
        \\.#..^.....
        \\........#.
        \\#.........
        \\......#...
    ;

    var floor = try Floor.init(example, std.testing.allocator);
    defer floor.deinit();

    try std.testing.expectEqual(41, try floor.part1());
    try std.testing.expectEqual(6, try floor.part2());
}
