const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/2024/day/15
    std.debug.print("Day 15, 2024: Warehouse Woes\n", .{});

    const m = utils.Monitor.init();
    defer m.deinit();
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const ally = gpa.allocator();
    const input = comptime utils.trim_input(@embedFile("input"));

    const p1 = try part1(input, ally);
    std.debug.print("  Part 1: {}\n", .{p1});
    std.debug.assert(1476771 == p1);

    const p2 = try part2(input, ally);
    std.debug.print("  Part 2: {}\n", .{p2});
    std.debug.assert(1468005 == p2);
}

const Int = i64;
const Point2d = utils.Point2d(Int);

const Map = struct {
    grid: std.AutoHashMap(Point2d, u8),
    instructions: []const u8,
    ally: std.mem.Allocator,
    width: Int,
    height: Int,
    robot: Point2d,

    fn init(comptime double: bool, input: []const u8, ally: std.mem.Allocator) !Map {
        var lineIt = std.mem.splitAny(u8, input, "\r\n");

        var grid = std.AutoHashMap(Point2d, u8).init(ally);
        var ins = std.ArrayList(u8).init(ally);
        var robot: Point2d = undefined;
        var y: Int = 0;
        var w: Int = 0;
        var h: Int = 0;
        var parsingMap = true;

        const xMultiplier: Int = if (double) 2 else 1;

        while (lineIt.next()) |line| : (y += 1) {
            if (parsingMap) {
                if (line.len == 0) {
                    h = y;
                    parsingMap = false;
                    continue;
                }

                w = @intCast(line.len);
                for (0.., line) |xx, c| {
                    const x: Int = @intCast(xx);
                    const p = Point2d{ .x = x * xMultiplier, .y = y };

                    try grid.put(p, c);
                    if (double) {
                        switch (c) {
                            '.', '#' => try grid.put(p.add(1, 0), c),
                            'O' => {
                                try grid.put(p, '[');
                                try grid.put(p.add(1, 0), ']');
                            },
                            '@' => try grid.put(p.add(1, 0), '.'),
                            else => {},
                        }
                    }

                    if (c == '@') {
                        robot = p;
                    }
                }
            } else {
                for (line) |c| {
                    try ins.append(c);
                }
            }
        }

        try grid.put(robot, '.');
        return .{
            .grid = grid,
            .instructions = try ins.toOwnedSlice(),
            .ally = ally,
            .width = w * xMultiplier,
            .height = h,
            .robot = robot,
        };
    }

    fn deinit(self: *@This()) void {
        self.grid.deinit();
        self.ally.free(self.instructions);
    }

    fn canPush(self: *@This(), boxes: []const Point2d, dir: Point2d) bool {
        for (boxes) |box| {
            const dest = box.addp(dir);
            const obj = self.grid.get(dest).?;

            switch (obj) {
                '#' => return false,
                'O' => {
                    if (!self.canPush(&.{dest}, dir)) {
                        return false;
                    }
                },
                '[' => {
                    if (dir.y == 0) {
                        if (!self.canPush(&.{dest}, dir)) {
                            return false;
                        }
                    } else if (!self.canPush(&.{ dest, dest.add(1, 0) }, dir)) {
                        return false;
                    }
                },
                ']' => {
                    if (dir.y == 0) {
                        if (!self.canPush(&.{dest}, dir)) {
                            return false;
                        }
                    } else if (!self.canPush(&.{ dest.add(-1, 0), dest }, dir)) {
                        return false;
                    }
                },
                '.' => {},
                else => unreachable,
            }
        }

        return true;
    }

    fn push(self: *@This(), boxes: []const Point2d, dir: Point2d) !void {
        for (boxes) |box| {
            const dest = box.addp(dir);
            const obj = self.grid.get(dest).?;

            switch (obj) {
                'O' => {
                    try self.push(&.{dest}, dir);
                },
                '[' => {
                    if (dir.y == 0) {
                        try self.push(&.{dest}, dir);
                    } else {
                        try self.push(&.{ dest, dest.add(1, 0) }, dir);
                    }
                },
                ']' => {
                    if (dir.y == 0) {
                        try self.push(&.{dest}, dir);
                    } else {
                        try self.push(&.{ dest.add(-1, 0), dest }, dir);
                    }
                },
                '.' => {},
                else => unreachable,
            }
        }

        for (boxes) |box| {
            const dest = box.addp(dir);
            const obj = self.grid.get(box).?;
            try self.grid.put(dest, obj);
            try self.grid.put(box, '.');
        }
    }

    fn move(self: *@This(), dir: Point2d) !void {
        const destination = self.robot.addp(dir);

        if (self.grid.get(destination)) |d| {
            switch (d) {
                '.', '@' => self.robot = destination,
                'O' => {
                    const objs = .{destination};
                    if (self.canPush(&objs, dir)) {
                        try self.push(&objs, dir);
                        self.robot = destination;
                    }
                },
                '[' => {
                    if (dir.y == 0) {
                        const objs = .{destination};
                        if (self.canPush(&objs, dir)) {
                            try self.push(&objs, dir);
                            self.robot = destination;
                        }
                    } else {
                        const objs = .{ destination, destination.add(1, 0) };
                        if (self.canPush(&objs, dir)) {
                            try self.push(&objs, dir);
                            self.robot = destination;
                        }
                    }
                },
                ']' => {
                    if (dir.y == 0) {
                        const objs = .{destination};
                        if (self.canPush(&objs, dir)) {
                            try self.push(&objs, dir);
                            self.robot = destination;
                        }
                    } else {
                        const objs = .{ destination.add(-1, 0), destination };
                        if (self.canPush(&objs, dir)) {
                            try self.push(&objs, dir);
                            self.robot = destination;
                        }
                    }
                },
                '#' => {},
                else => unreachable,
            }
        }
    }

    fn step(self: *@This(), i: u8) !void {
        switch (i) {
            '^' => try self.move(.{ .x = 0, .y = -1 }),
            '>' => try self.move(.{ .x = 1, .y = 0 }),
            '<' => try self.move(.{ .x = -1, .y = 0 }),
            'v' => try self.move(.{ .x = 0, .y = 1 }),
            else => unreachable,
        }
    }
    fn run(self: *@This()) !void {
        for (self.instructions) |i| {
            try self.step(i);
        }
    }

    fn score(self: *@This()) Int {
        var result: Int = 0;
        var it = self.grid.iterator();
        while (it.next()) |entry| {
            if (entry.value_ptr.* == 'O' or entry.value_ptr.* == '[') {
                const p = entry.key_ptr.*;
                result += p.x + p.y * 100;
            }
        }

        return result;
    }

    fn print(self: *@This()) void {
        var p = Point2d{ .x = 0, .y = 0 };
        while (p.y != self.height) : (p.y += 1) {
            p.x = 0;
            while (p.x != self.width) : (p.x += 1) {
                if (p.eql(self.robot)) {
                    std.debug.print("@", .{});
                } else {
                    std.debug.print("{c}", .{self.grid.get(p) orelse '?'});
                }
            }
            std.debug.print("\n", .{});
        }
    }
};

fn part1(input: []const u8, ally: std.mem.Allocator) !Int {
    var map = try Map.init(false, input, ally);
    defer map.deinit();
    try map.run();
    return map.score();
}

fn part2(input: []const u8, ally: std.mem.Allocator) !Int {
    var map = try Map.init(true, input, ally);
    defer map.deinit();
    try map.run();
    // map.print();
    return map.score();
}

test "parts 1,2" {
    const small =
        \\########
        \\#..O.O.#
        \\##@.O..#
        \\#...O..#
        \\#.#.O..#
        \\#...O..#
        \\#......#
        \\########
        \\
        \\<^^>>>vv<v>>v<<
    ;
    const example =
        \\##########
        \\#..O..O.O#
        \\#......O.#
        \\#.OO..O.O#
        \\#..O@..O.#
        \\#O#..O...#
        \\#O..O..O.#
        \\#.OO.O.OO#
        \\#....O...#
        \\##########
        \\
        \\<vv>^<v^>v>^vv^v>v<>v^v<v<^vv<<<^><<><>>v<vvv<>^v^>^<<<><<v<<<v^vv^v>^
        \\vvv<<^>^v^^><<>>><>^<<><^vv^^<>vvv<>><^^v>^>vv<>v<<<<v<^v>^<^^>>>^<v<v
        \\><>vv>v^v^<>><>>>><^^>vv>v<^^^>>v^v^<^^>v^^>v^<^v>v<>>v^v^<v>v^^<^^vv<
        \\<<v<^>>^^^^>>>v^<>vvv^><v<<<>^^^vv^<vvv>^>v<^^^^v<>^>vvvv><>>v^<<^^^^^
        \\^><^><>>><>^^<<^^v>>><^<v>^<vv>>v>>>^v><>^v><<<<v>>v<v<v>vvv>^<><<>^><
        \\^>><>^v<><^vvv<^^<><v<<<<<><^v<<<><<<^^<v<^^^><^>>^<v^><<<^>>^v<v^v<v^
        \\>^>>^v>vv>^<<^v<>><<><<v<<v><>v<^vv<<<>^^v^>^^>>><<^v>>v^v><^^>>^<>vv^
        \\<><^^>^^^<><vvvvv^v<v<<>^v<v>v<<^><<><<><<<^^<<<^<<>><<><^^^>^^<>^>v<>
        \\^^>vv<^v^v<vv>^<><v<^v>^^^>>>^^vvv^>vvv<>>>^<^>>>>>^<<^v>^vvv<>^<><<v>
        \\v^^>>><<^^<>>^v^<v^vv<>v^<<>^<^v^v><^<<<><<^<v><v<>vv>>v><v^<vv<>v^<<^
    ;
    const mine =
        \\##########
        \\#........#
        \\#........#
        \\#........#
        \\#...#....#
        \\#........#
        \\#.OOOOOO.#
        \\#.@OOOO..#
        \\#...OOO..#
        \\#........#
        \\##########
        \\
        \\>>v>v>^^^^
    ;

    try std.testing.expectEqual(2028, try part1(small, std.testing.allocator));
    try std.testing.expectEqual(10092, try part1(example, std.testing.allocator));
    try std.testing.expectEqual(7770, try part1(mine, std.testing.allocator));
    try std.testing.expectEqual(1751, try part2(small, std.testing.allocator));
    try std.testing.expectEqual(9021, try part2(example, std.testing.allocator));
    try std.testing.expectEqual(8324, try part2(mine, std.testing.allocator));
}
