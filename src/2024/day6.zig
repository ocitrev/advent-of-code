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
    map: std.AutoHashMap(Point2d, u8),
    visited: std.AutoHashMap(Point2d, void),
    visited_dir: std.AutoHashMap([2]Point2d, void),
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
            .map = map,
            .visited = std.AutoHashMap(Point2d, void).init(ally),
            .visited_dir = std.AutoHashMap([2]Point2d, void).init(ally),
            .guard = guard,
        };
    }

    pub fn deinit(self: *@This()) void {
        self.map.deinit();
        self.visited.deinit();
        self.visited_dir.deinit();
    }

    fn run(self: *@This()) !void {
        var g = self.guard;
        var dir = Point2d{ .x = 0, .y = -1 };
        while (true) {
            try self.visited.put(g, {});
            const newPos = g.add(dir.x, dir.y);

            if (self.map.get(newPos)) |cell| {
                if (cell == '#') {
                    dir = dir.rotate90Right();
                    continue;
                }

                g = newPos;
            } else {
                return;
            }
        }
    }

    fn runWithObstacle(self: *@This(), obstacle: Point2d) !bool {
        var g = self.guard;
        var dir = Point2d{ .x = 0, .y = -1 };
        self.visited_dir.clearRetainingCapacity();

        while (true) {
            if (self.visited_dir.contains(.{ g, dir })) {
                return true;
            }
            try self.visited_dir.put(.{ g, dir }, {});
            const new_pos = g.add(dir.x, dir.y);

            if (self.map.get(new_pos)) |cell| {
                if (cell == '#' or obstacle.eql(new_pos)) {
                    dir = dir.rotate90Right();
                    continue;
                }

                g = new_pos;
            } else {
                return false;
            }
        }
    }

    pub fn part1(self: *@This()) !i32 {
        try self.run();
        return @intCast(self.visited.count());
    }

    pub fn part2(self: *@This()) !i32 {
        var sum: i32 = 0;

        var it = self.visited.keyIterator();
        while (it.next()) |p| {
            if (self.guard.eql(p.*)) {
                continue;
            }
            if (try self.runWithObstacle(p.*)) {
                sum += 1;
            }
        }

        return sum;
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
