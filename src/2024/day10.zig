const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/2024/day/10
    std.debug.print("Day 10, 2024: Hoof It\n", .{});

    const m = utils.Monitor.init();
    defer m.deinit();
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const ally = gpa.allocator();
    const input = comptime utils.trim_input(@embedFile("input"));

    const p1 = try part1(input, ally);
    std.debug.print("  Part 1: {}\n", .{p1});
    std.debug.assert(587 == p1);

    const p2 = try part2(input, ally);
    std.debug.print("  Part 2: {}\n", .{p2});
    std.debug.assert(1340 == p2);
}

const Point2d = utils.Point2d(i32);

const Map = struct {
    grid: std.AutoHashMap(Point2d, u8),
    heads: std.ArrayList(Point2d),
    ends: std.ArrayList(Point2d),
    ally: std.mem.Allocator,

    fn init(input: []const u8, ally: std.mem.Allocator) !Map {
        var result = Map{
            .grid = std.AutoHashMap(Point2d, u8).init(ally),
            .heads = std.ArrayList(Point2d).init(ally),
            .ends = std.ArrayList(Point2d).init(ally),
            .ally = ally,
        };

        var y: i32 = 0;
        var it = std.mem.tokenizeAny(u8, input, "\r\n");
        while (it.next()) |line| : (y += 1) {
            for (0.., line) |x, c| {
                const p = Point2d{ .x = @intCast(x), .y = y };

                switch (c) {
                    '0'...'9' => try result.grid.put(p, try std.fmt.charToDigit(c, 10)),
                    '.' => try result.grid.put(p, 255),
                    else => {},
                }

                switch (c) {
                    '0' => try result.heads.append(p),
                    '9' => try result.ends.append(p),
                    else => {},
                }
            }
        }
        return result;
    }

    fn deinit(self: *@This()) void {
        self.grid.deinit();
        self.heads.deinit();
        self.ends.deinit();
    }

    fn contains(self: *const @This(), p: Point2d(i32)) bool {
        return self.grid.contains(p);
    }

    fn getScoreRecursive(self: *@This(), p: Point2d, visited: *std.AutoHashMap(Point2d, void)) i32 {
        const height: u8 = self.grid.get(p).?;
        var score: i32 = 0;

        if (visited.*.contains(p)) return 0;
        visited.*.put(p, {}) catch unreachable;

        if (height == 9) {
            return 1;
        }

        const north = p.north();
        if (self.grid.get(north)) |h| {
            if (h == height + 1) {
                score += self.getScoreRecursive(north, visited);
            }
        }

        const west = p.west();
        if (self.grid.get(west)) |h| {
            if (h == height + 1) {
                score += self.getScoreRecursive(west, visited);
            }
        }

        const east = p.east();
        if (self.grid.get(east)) |h| {
            if (h == height + 1) {
                score += self.getScoreRecursive(east, visited);
            }
        }

        const south = p.south();
        if (self.grid.get(south)) |h| {
            if (h == height + 1) {
                score += self.getScoreRecursive(south, visited);
            }
        }

        return score;
    }

    fn getScores(self: *@This()) i32 {
        var sum: i32 = 0;
        var visited = std.AutoHashMap(Point2d, void).init(self.ally);
        defer visited.deinit();

        for (self.heads.items) |head| {
            visited.clearRetainingCapacity();
            sum += self.getScoreRecursive(head, &visited);
        }
        return sum;
    }

    fn getDistinctRatingRecursive(self: *@This(), p: Point2d) i32 {
        const height: u8 = self.grid.get(p).?;
        var score: i32 = 0;

        if (height == 0) {
            return 1;
        }

        const north = p.north();
        if (self.grid.get(north)) |h| {
            if (h == height - 1) {
                score += self.getDistinctRatingRecursive(north);
            }
        }

        const west = p.west();
        if (self.grid.get(west)) |h| {
            if (h == height - 1) {
                score += self.getDistinctRatingRecursive(west);
            }
        }

        const east = p.east();
        if (self.grid.get(east)) |h| {
            if (h == height - 1) {
                score += self.getDistinctRatingRecursive(east);
            }
        }

        const south = p.south();
        if (self.grid.get(south)) |h| {
            if (h == height - 1) {
                score += self.getDistinctRatingRecursive(south);
            }
        }

        return score;
    }

    fn getDistinctRatings(self: *@This()) i32 {
        var sum: i32 = 0;
        for (self.ends.items) |end| {
            sum += self.getDistinctRatingRecursive(end);
        }
        return sum;
    }
};

fn part1(input: []const u8, ally: std.mem.Allocator) !i32 {
    var map = try Map.init(input, ally);
    defer map.deinit();
    return map.getScores();
}

test "part 1" {
    const example =
        \\89010123
        \\78121874
        \\87430965
        \\96549874
        \\45678903
        \\32019012
        \\01329801
        \\10456732
    ;
    try std.testing.expectEqual(36, try part1(example, std.testing.allocator));
}

fn part2(input: []const u8, ally: std.mem.Allocator) !i32 {
    var map = try Map.init(input, ally);
    defer map.deinit();
    return map.getDistinctRatings();
}

test "part 2" {
    const simple =
        \\.....0.
        \\..4321.
        \\..5..2.
        \\..6543.
        \\..7..4.
        \\..8765.
        \\..9....
    ;
    try std.testing.expectEqual(3, try part2(simple, std.testing.allocator));

    const complex =
        \\012345
        \\123456
        \\234567
        \\345678
        \\4.6789
        \\56789.
    ;
    try std.testing.expectEqual(227, try part2(complex, std.testing.allocator));

    const example =
        \\89010123
        \\78121874
        \\87430965
        \\96549874
        \\45678903
        \\32019012
        \\01329801
        \\10456732
    ;
    try std.testing.expectEqual(81, try part2(example, std.testing.allocator));
}
