const std = @import("std");
const utils = @import("utils");
const Point2d = utils.Point2d(i32);

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const ally = gpa.allocator();

    const input = comptime utils.trim_input(@embedFile("input"));
    // https://adventofcode.com/2024/day/6
    std.debug.print("Day 6, 2024: \n", .{});

    const p1 = try part1(input, ally);
    std.debug.print("  Part 1: {}\n", .{p1});
    std.debug.assert(5199 == p1);

    const p2 = try part2(input, ally);
    std.debug.print("  Part 2: {}\n", .{p2});
    std.debug.assert(1915 == p2);
}

fn findGuard(grid: *const utils.Grid) Point2d {
    var it = grid.map.iterator();

    while (it.next()) |v| {
        if (v.value_ptr.* == '^') {
            return v.key_ptr.*;
        }
    }

    unreachable;
}

fn run(
    grid: *const utils.Grid,
    obstacle: ?Point2d,
    context: anytype,
    comptime visited: fn (@TypeOf(context), Point2d, Point2d) bool,
) void {
    var dir = Point2d{ .x = 0, .y = -1 };
    var g = findGuard(grid);
    while (true) {
        if (!visited(context, g, dir)) {
            return;
        }

        const newPos = g.add(dir.x, dir.y);

        if (grid.map.get(newPos)) |cell| {
            if (cell == '#' or (obstacle != null and newPos.eql(obstacle.?))) {
                dir = dir.rotate90Right();
                continue;
            }

            g = newPos;
        } else {
            return;
        }
    }
}

fn insertGuard(visited: *std.AutoHashMap(Point2d, void), guard: Point2d, dir: Point2d) bool {
    _ = dir;
    visited.put(guard, {}) catch return false;
    return true;
}

fn part1(input: []const u8, ally: std.mem.Allocator) !i32 {
    var grid = try utils.Grid.parse(input, ally);
    defer grid.deinit();

    var visited = std.AutoHashMap(Point2d, void).init(ally);
    defer visited.deinit();
    run(&grid, null, &visited, insertGuard);
    return @intCast(visited.count());
}

test "part 1" {
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
    try std.testing.expectEqual(41, try part1(example, std.testing.allocator));
}

const Visited = struct {
    p: Point2d,
    d: Point2d,
};

const Context = struct {
    visited: *std.AutoHashMap(Visited, void),
    sum: *i32,
};

fn insertGuardAndDir(ctx: Context, g: Point2d, d: Point2d) bool {
    const v = Visited{ .p = g, .d = d };

    if (ctx.visited.contains(v)) {
        ctx.sum.* += 1;
        return false;
    }

    ctx.visited.put(v, {}) catch return false;
    return true;
}

fn part2(input: []const u8, ally: std.mem.Allocator) !i32 {
    var grid = try utils.Grid.parse(input, ally);
    defer grid.deinit();

    var cur = Point2d{};
    var sum: i32 = 0;
    while (cur.y < grid.height) : (cur.y += 1) {
        cur.x = 0;
        while (cur.x < grid.width) : (cur.x += 1) {
            var visited = std.AutoHashMap(Visited, void).init(ally);
            defer visited.deinit();
            const ctx = Context{ .visited = &visited, .sum = &sum };
            run(&grid, cur, ctx, insertGuardAndDir);
        }
    }

    return sum;
}

test "part 2" {
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
    try std.testing.expectEqual(6, try part2(example, std.testing.allocator));
}
