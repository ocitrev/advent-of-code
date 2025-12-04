const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/2025/day/4
    utils.printTitle(2025, 4, "Printing Department");

    const m = utils.Monitor.init();
    defer m.deinit();
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const ally = gpa.allocator();
    const input = comptime utils.trimInput(@embedFile("input"));

    const p1 = try part1(ally, input);
    utils.printAnswer(1, p1);
    std.debug.assert(1457 == p1);

    const p2 = try part2(ally, input);
    utils.printAnswer(2, p2);
    std.debug.assert(8310 == p2);
}

const Int = i32;
const Point2d = utils.Point2d(Int);

const Map = struct {
    grid: std.AutoHashMap(Point2d, u8),
    width: Int,
    height: Int,
    ally: std.mem.Allocator,

    pub fn parse(ally: std.mem.Allocator, input: []const u8) !Map {
        var grid = std.AutoHashMap(Point2d, u8).init(ally);
        var height: Int = 0;
        var width: Int = 0;

        var lineIt = std.mem.tokenizeAny(u8, input, "\r\n");
        while (lineIt.next()) |line| {
            var x: Int = 0;
            for (line) |c| {
                const p = Point2d{ .x = x, .y = height };
                try grid.put(p, c);
                x += 1;
                width = @max(width, x);
            }

            height += 1;
        }

        return Map{
            .grid = grid,
            .width = width,
            .height = height,
            .ally = ally,
        };
    }

    pub fn removeAccessible(self: *@This()) !Int {
        var toRemove = std.ArrayList(Point2d).empty;
        defer toRemove.deinit(self.ally);

        var it = self.grid.iterator();
        while (it.next()) |entry| {
            if (entry.value_ptr.* != '@') continue;

            const p = entry.key_ptr.*;
            var nbAround: Int = 0;

            if (self.grid.get(p.north().west()) == '@') nbAround += 1;
            if (self.grid.get(p.north()) == '@') nbAround += 1;
            if (self.grid.get(p.north().east()) == '@') nbAround += 1;
            if (self.grid.get(p.east()) == '@') nbAround += 1;
            if (self.grid.get(p.west()) == '@') nbAround += 1;
            if (self.grid.get(p.south().west()) == '@') nbAround += 1;
            if (self.grid.get(p.south()) == '@') nbAround += 1;
            if (self.grid.get(p.south().east()) == '@') nbAround += 1;

            if (nbAround < 4) {
                try toRemove.append(self.ally, p);
            }
        }

        for (toRemove.items) |p| {
            try self.grid.put(p, '.');
        }

        return @intCast(toRemove.items.len);
    }

    pub fn deinit(self: *@This()) void {
        self.grid.deinit();
    }
};

fn part1(ally: std.mem.Allocator, input: []const u8) !Int {
    var map = try Map.parse(ally, input);
    defer map.deinit();
    return try map.removeAccessible();
}

fn part2(ally: std.mem.Allocator, input: []const u8) !Int {
    var map = try Map.parse(ally, input);
    defer map.deinit();

    var toRemove = std.ArrayList(Point2d).empty;
    defer toRemove.deinit(ally);
    var totalRemoved: Int = 0;

    while (true) {
        const removed = try map.removeAccessible();
        if (removed == 0) break;
        totalRemoved += removed;
    }

    return totalRemoved;
}

test "parts 1,2" {
    const ally = std.testing.allocator;
    const example =
        \\..@@.@@@@.
        \\@@@.@.@.@@
        \\@@@@@.@.@@
        \\@.@@@@..@.
        \\@@.@@@@.@@
        \\.@@@@@@@.@
        \\.@.@.@.@@@
        \\@.@@@.@@@@
        \\.@@@@@@@@.
        \\@.@.@@@.@.
    ;
    try std.testing.expectEqual(13, try part1(ally, example));
    try std.testing.expectEqual(43, try part2(ally, example));
}
