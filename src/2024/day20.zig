const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/2024/day/20
    utils.printTitle(2024, 20, "Race Condition");

    const m = utils.Monitor.init();
    defer m.deinit();
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const ally = gpa.allocator();
    const input = comptime utils.trimInput(@embedFile("input"));

    const p1 = try part1(ally, input, 100);
    utils.printAnswer(1, p1);
    std.debug.assert(1507 == p1);

    const p2 = try part2(ally, input, 100);
    utils.printAnswer(2, p2);
    std.debug.assert(1037936 == p2);
}

const Int = i32;
const Point2d = utils.Point2d(Int);

const Node = struct {
    p: Point2d,
    cost: Int,
};

const Cheat = struct {
    from: Point2d,
    to: Point2d,
    len: Int,
};

const Grid = struct {
    ally: std.mem.Allocator,
    map: std.AutoHashMap(Point2d, u8),
    parents: std.AutoHashMap(Point2d, Node),
    cheats: std.ArrayList(Cheat),
    start: Point2d,
    end: Point2d,
    width: Int,
    height: Int,

    fn init(ally: std.mem.Allocator, input: []const u8) !Grid {
        var map = std.AutoHashMap(Point2d, u8).init(ally);
        var start: Point2d = undefined;
        var end: Point2d = undefined;
        var w: Int = undefined;
        var y: Int = 0;
        var lineIt = std.mem.tokenizeAny(u8, input, "\r\n");
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
            .parents = std.AutoHashMap(Point2d, Node).init(ally),
            .cheats = std.ArrayList(Cheat).empty,
            .start = start,
            .end = end,
            .width = w,
            .height = y,
        };
    }

    fn deinit(self: *@This()) void {
        self.map.deinit();
        self.parents.deinit();
        self.cheats.deinit(self.ally);
    }

    fn getCheat(self: *@This(), c: Cheat) Int {
        const costFrom = if (self.parents.get(c.from)) |f| f.cost else 0;
        const costTo = if (self.parents.get(c.to)) |t| t.cost else 0;
        const len = costTo - costFrom;
        return len - c.len;
    }

    fn solve(self: *@This(), maxCheat: Int) !void {
        var q = std.ArrayList(Node).empty;
        defer q.deinit(self.ally);
        var visited = std.AutoHashMap(Point2d, void).init(self.ally);
        defer visited.deinit();

        try q.append(self.ally, .{ .p = self.start, .cost = 1 });
        while (q.items.len != 0) {
            const node = q.orderedRemove(0);
            try visited.put(node.p, {});

            const left = node.p.x - maxCheat;
            const right = node.p.x + maxCheat;
            const top = node.p.y - maxCheat;
            const bottom = node.p.y + maxCheat;
            var y = top;

            while (y <= bottom) : (y += 1) {
                var x = left;
                while (x <= right) : (x += 1) {
                    const p = Point2d{ .x = @intCast(x), .y = @intCast(y) };
                    const cheatLen = node.p.manhatanDistance(p);
                    if (cheatLen < 2 or cheatLen > maxCheat or self.map.get(p) != '.') {
                        continue;
                    }

                    try self.cheats.append(self.ally, .{
                        .from = node.p,
                        .to = p,
                        .len = cheatLen,
                    });
                }
            }

            const dirs = [_]Point2d{
                Point2d.NORTH,
                Point2d.SOUTH,
                Point2d.EAST,
                Point2d.WEST,
            };

            for (dirs) |d| {
                const next = node.p.addp(d);
                if (visited.contains(next)) {
                    continue;
                }
                if (self.map.get(next)) |c| {
                    if (c == '.') {
                        const newNode = Node{ .p = next, .cost = node.cost + 1 };
                        try q.append(self.ally, newNode);
                        try self.parents.put(next, node);
                    }
                }
            }
        }
    }

    fn countCheats(self: *@This(), saveTarget: Int) Int {
        var sum: Int = 0;
        for (self.cheats.items) |c| {
            const saved = self.getCheat(c);
            if (saved == saveTarget) {
                sum += 1;
            }
        }
        return sum;
    }
};

fn part1(ally: std.mem.Allocator, input: []const u8, minTimeSave: Int) !Int {
    var grid = try Grid.init(ally, input);
    defer grid.deinit();
    try grid.solve(2);

    var sum: Int = 0;
    for (grid.cheats.items) |c| {
        const saved = grid.getCheat(c);
        if (saved >= minTimeSave) {
            sum += 1;
        }
    }

    return sum;
}

const showSummary = false;

fn part2(ally: std.mem.Allocator, input: []const u8, minTimeSave: Int) !Int {
    var grid = try Grid.init(ally, input);
    defer grid.deinit();
    try grid.solve(20);

    var summary = std.AutoHashMap(Int, Int).init(ally);
    defer summary.deinit();

    var maximumSave: ?Int = null;
    var sum: Int = 0;
    for (grid.cheats.items) |c| {
        const saved = grid.getCheat(c);
        maximumSave = @max(maximumSave orelse saved, saved);

        if (saved >= minTimeSave) {
            if (showSummary) {
                if (summary.get(saved)) |existing| {
                    try summary.put(saved, existing + 1);
                } else {
                    try summary.put(saved, 1);
                }
            }

            sum += 1;
        }
    }

    if (showSummary) {
        std.debug.print("Summary:\n", .{});

        var i: Int = minTimeSave;
        while (i <= maximumSave.?) : (i += 1) {
            if (summary.get(i)) |v| {
                std.debug.print("  There are {} cheats that save {} picoseconds.\n", .{ v, i });
            }
        }
    }

    return sum;
}

test "part 1, 2" {
    const ally = std.testing.allocator;
    const example =
        \\###############
        \\#...#...#.....#
        \\#.#.#.#.#.###.#
        \\#S#...#.#.#...#
        \\#######.#.#.###
        \\#######.#.#...#
        \\#######.#.###.#
        \\###..E#...#...#
        \\###.#######.###
        \\#...###...#...#
        \\#.#####.#.###.#
        \\#.#...#.#.#...#
        \\#.#.#.#.#.#.###
        \\#...#...#...###
        \\###############
    ;

    try std.testing.expectEqual(44, try part1(ally, example, 2));

    var grid = try Grid.init(ally, example);
    defer grid.deinit();
    try grid.solve(20);

    try std.testing.expectEqual(32, grid.countCheats(50));
    try std.testing.expectEqual(31, grid.countCheats(52));
    try std.testing.expectEqual(29, grid.countCheats(54));
    try std.testing.expectEqual(39, grid.countCheats(56));
    try std.testing.expectEqual(25, grid.countCheats(58));
    try std.testing.expectEqual(23, grid.countCheats(60));
    try std.testing.expectEqual(20, grid.countCheats(62));
    try std.testing.expectEqual(19, grid.countCheats(64));
    try std.testing.expectEqual(12, grid.countCheats(66));
    try std.testing.expectEqual(14, grid.countCheats(68));
    try std.testing.expectEqual(12, grid.countCheats(70));
    try std.testing.expectEqual(22, grid.countCheats(72));
    try std.testing.expectEqual(4, grid.countCheats(74));
    try std.testing.expectEqual(3, grid.countCheats(76));
}
