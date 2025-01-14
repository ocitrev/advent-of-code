const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/2024/day/16
    utils.printTitle(2024, 16, "Reindeer Maze");

    const m = utils.Monitor.init();
    defer m.deinit();
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const ally = gpa.allocator();
    const input = comptime utils.trimInput(@embedFile("input"));

    var grid = try Grid.init(ally, input);
    defer grid.deinit();

    const p1 = try grid.part1();
    utils.printAnswer(1, p1);
    std.debug.assert(88416 == p1);

    const p2 = try grid.part2();
    utils.printAnswer(2, p2);
    std.debug.assert(442 == p2);
}

const Int = i32;
const Point2d = utils.Point2d(Int);

const Elem = struct {
    pos: Point2d,
    dir: Point2d,
};

const Node = struct {
    e: Elem,
    cost: Int,
};

const dirs = [_]Point2d{
    Point2d.NORTH,
    Point2d.SOUTH,
    Point2d.EAST,
    Point2d.WEST,
};

const Grid = struct {
    ally: std.mem.Allocator,
    map: std.AutoHashMap(Point2d, u8),
    start: Point2d,
    end: Point2d,
    width: Int,
    height: Int,
    cacheForward: std.AutoHashMap(Elem, Int),
    cacheBackward: std.AutoHashMap(Elem, Int),
    best: ?Int = null,

    fn init(ally: std.mem.Allocator, input: []const u8) !Grid {
        var map = std.AutoHashMap(Point2d, u8).init(ally);

        var w: Int = 0;
        var lineIt = std.mem.tokenizeAny(u8, input, "\r\n");
        var start: Point2d = undefined;
        var end: Point2d = undefined;
        var y: Int = 0;
        while (lineIt.next()) |line| : (y += 1) {
            for (0.., line) |x, c| {
                const p = Point2d{ .x = @intCast(x), .y = y };

                const b = switch (c) {
                    'S' => blk: {
                        start = p;
                        break :blk '.';
                    },
                    'E' => blk: {
                        end = p;
                        break :blk '.';
                    },
                    else => c,
                };

                try map.put(p, b);
            }

            w = @intCast(line.len);
        }

        return .{
            .ally = ally,
            .map = map,
            .start = start,
            .end = end,
            .width = w,
            .height = y,
            .cacheForward = std.AutoHashMap(Elem, Int).init(ally),
            .cacheBackward = std.AutoHashMap(Elem, Int).init(ally),
        };
    }

    fn deinit(self: *@This()) void {
        self.map.deinit();
        self.cacheForward.deinit();
        self.cacheBackward.deinit();
    }

    fn shortestPathForward(self: *@This()) void {
        self.shortestPath(self.start, Point2d.EAST, self.end, false);
    }

    fn shortestPathBackward(self: *@This()) void {
        self.shortestPath(self.end, null, null, true);
    }

    fn shortestPath(
        self: *@This(),
        start: Point2d,
        startDir: ?Point2d,
        end: ?Point2d,
        comptime reverse: bool,
    ) void {
        const cmp = struct {
            fn call(_: void, a: Node, b: Node) std.math.Order {
                return std.math.order(a.cost, b.cost);
            }
        }.call;

        var q = std.PriorityQueue(Node, void, cmp).init(self.ally, {});
        defer q.deinit();

        var visited = std.AutoHashMap(Elem, void).init(self.ally);
        defer visited.deinit();

        if (startDir) |d| {
            const startNode = .{
                .e = .{ .pos = start, .dir = d },
                .cost = 0,
            };
            q.add(startNode) catch unreachable;
        } else {
            for (dirs) |d| {
                const startNode = .{
                    .e = .{ .pos = start, .dir = d },
                    .cost = 0,
                };
                q.add(startNode) catch unreachable;
            }
        }

        var cache = if (reverse) &self.cacheBackward else &self.cacheForward;

        while (q.items.len != 0) {
            const node = q.remove();

            if (!cache.contains(node.e)) {
                cache.putNoClobber(node.e, node.cost) catch unreachable;
            }

            if (end) |goal| {
                if (node.e.pos.eql(goal)) {
                    self.best = node.cost;
                    return;
                }
            }

            if (visited.fetchPut(node.e, {}) catch unreachable != null) {
                continue;
            }

            const d = node.e.dir;
            const newPos = node.e.pos.addp(d);
            if (self.map.get(newPos) == '.') {
                q.add(.{
                    .e = .{ .pos = newPos, .dir = d },
                    .cost = node.cost + 1,
                }) catch unreachable;
            }

            q.add(.{
                .e = .{ .pos = node.e.pos, .dir = d.rotate90Left() },
                .cost = node.cost + 1000,
            }) catch unreachable;

            q.add(.{
                .e = .{ .pos = node.e.pos, .dir = d.rotate90Right() },
                .cost = node.cost + 1000,
            }) catch unreachable;
        }
    }

    fn part1(self: *@This()) !Int {
        if (self.best == null) {
            self.shortestPathForward();
        }
        return self.best orelse 0;
    }

    fn getCost(self: *@This(), comptime reverse: bool, p: Point2d, d: Point2d) Int {
        if (reverse) {
            const elm = Elem{ .pos = p, .dir = d.neg() };
            return self.cacheBackward.get(elm) orelse 0;
        }

        const elm = Elem{ .pos = p, .dir = d };
        return self.cacheForward.get(elm) orelse 0;
    }

    fn part2(self: *@This()) !Int {
        if (self.best == null) {
            self.shortestPathForward();
        }

        self.shortestPathBackward();
        var points = std.AutoHashMap(Point2d, void).init(self.ally);
        defer points.deinit();

        var it = self.map.keyIterator();
        while (it.next()) |p| {
            for (dirs) |d| {
                if (self.getCost(false, p.*, d) + self.getCost(true, p.*, d) == self.best) {
                    try points.put(p.*, {});
                }
            }
        }

        return @intCast(points.count());
    }
};

test "parts 1,2" {
    const ally = std.testing.allocator;
    const example1 =
        \\###############
        \\#.......#....E#
        \\#.#.###.#.###.#
        \\#.....#.#...#.#
        \\#.###.#####.#.#
        \\#.#.#.......#.#
        \\#.#.#####.###.#
        \\#...........#.#
        \\###.#.#####.#.#
        \\#...#.....#.#.#
        \\#.#.#.###.#.#.#
        \\#.....#...#.#.#
        \\#.###.#.#.#.#.#
        \\#S..#.....#...#
        \\###############
    ;
    const example2 =
        \\#################
        \\#...#...#...#..E#
        \\#.#.#.#.#.#.#.#.#
        \\#.#.#.#...#...#.#
        \\#.#.#.#.###.#.#.#
        \\#...#.#.#.....#.#
        \\#.#.#.#.#.#####.#
        \\#.#...#.#.#.....#
        \\#.#.#####.#.###.#
        \\#.#.#.......#...#
        \\#.#.###.#####.###
        \\#.#.#...#.....#.#
        \\#.#.#.#####.###.#
        \\#.#.#.........#.#
        \\#.#.#.#########.#
        \\#S#.............#
        \\#################
    ;

    var grid1 = try Grid.init(ally, example1);
    defer grid1.deinit();
    try std.testing.expectEqual(7036, try grid1.part1());
    try std.testing.expectEqual(45, try grid1.part2());

    var grid2 = try Grid.init(ally, example2);
    defer grid2.deinit();
    try std.testing.expectEqual(11048, try grid2.part1());
    try std.testing.expectEqual(64, try grid2.part2());
}
