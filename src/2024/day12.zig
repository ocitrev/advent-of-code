const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/2024/day/12
    std.debug.print("Day 12, 2024: Garden Groups\n", .{});

    const m = utils.Monitor.init();
    defer m.deinit();
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const ally = gpa.allocator();
    const input = comptime utils.trim_input(@embedFile("input"));

    const p1 = try part1(input, ally);
    std.debug.print("  Part 1: {}\n", .{p1});
    std.debug.assert(1304764 == p1);

    const p2 = try part2(input, ally);
    std.debug.print("  Part 2: {}\n", .{p2});
    std.debug.assert(811148 == p2);
}

const Int = i32;
const Point2d = utils.Point2d(i32);
const PriceType = enum { perimeter, side };
const Region = struct {
    seed: u8,
    perimeter: Int = 0,
    area: Int = 0,
    sides: Int = 0,

    fn price(self: *const @This(), comptime priceType: PriceType) Int {
        return switch (priceType) {
            .perimeter => return self.perimeter * self.area,
            .side => return self.sides * self.area,
        };
    }
};

const SideType = enum {
    north,
    south,
    east,
    west,
};

const Side = struct {
    Point2d,
    SideType,
};

const Map = struct {
    grid: utils.Grid,
    regions: std.ArrayList(Region),
    visited: std.AutoHashMap(Point2d, void),
    ally: std.mem.Allocator,

    fn init(input: []const u8, ally: std.mem.Allocator) !Map {
        return Map{
            .grid = try utils.Grid.parse(input, ally),
            .regions = std.ArrayList(Region).init(ally),
            .visited = std.AutoHashMap(Point2d, void).init(ally),
            .ally = ally,
        };
    }

    fn deinit(self: *@This()) void {
        self.grid.deinit();
        self.regions.deinit();
        self.visited.deinit();
    }

    const Sides = utils.HashMapArray(SideType, Point2d);

    fn walkRecurse(self: *@This(), seed: u8, p: Point2d, region: *Region, sides: *Sides) !void {
        if (self.visited.contains(p)) return;
        try self.visited.put(p, void{});
        region.area += 1;

        const n = p.north();
        if (self.grid.map.get(n) == seed) {
            try self.walkRecurse(seed, n, region, sides);
        } else {
            try sides.put(.north, p);
            region.perimeter += 1;
        }

        const w = p.west();
        if (self.grid.map.get(w) == seed) {
            try self.walkRecurse(seed, w, region, sides);
        } else {
            try sides.put(.west, p);
            region.perimeter += 1;
        }

        const s = p.south();
        if (self.grid.map.get(s) == seed) {
            try self.walkRecurse(seed, s, region, sides);
        } else {
            try sides.put(.south, p);
            region.perimeter += 1;
        }

        const e = p.east();
        if (self.grid.map.get(e) == seed) {
            try self.walkRecurse(seed, e, region, sides);
        } else {
            try sides.put(.east, p);
            region.perimeter += 1;
        }
    }

    fn cmp(_: void, a: Point2d, b: Point2d) bool {
        if (a.y == b.y) {
            return a.x < b.x;
        }

        return a.y < b.y;
    }

    fn countSides(self: *@This(), sideType: SideType, sides: []const Point2d) !Int {
        if (sides.len == 1) {
            return 1;
        }

        const sorted = try self.ally.alloc(Point2d, sides.len);
        defer self.ally.free(sorted);

        switch (sideType) {
            .north, .south => std.mem.copyForwards(Point2d, sorted, sides),
            .east, .west => {
                for (0.., sides) |i, s| {
                    sorted[i] = Point2d{ .x = s.y, .y = s.x };
                }
            },
        }

        std.mem.sort(Point2d, sorted, {}, cmp);
        var sum: Int = 1;
        var it = std.mem.window(Point2d, sorted, 2, 1);

        while (it.next()) |x| {
            const a = x[0];
            const b = x[1];
            if (a.y != b.y or a.x != b.x - 1) {
                sum += 1;
            }
        }

        return sum;
    }

    fn walk(self: *@This()) !void {
        var gridIt = self.grid.map.iterator();
        while (gridIt.next()) |cell| {
            var sides = Sides.init(self.ally);
            defer sides.deinit();

            const seed = cell.value_ptr.*;
            const p = cell.key_ptr.*;
            var region = Region{ .seed = seed };
            try self.walkRecurse(seed, p, &region, &sides);
            if (sides.get(.north)) |s| region.sides += try self.countSides(.north, s);
            if (sides.get(.south)) |s| region.sides += try self.countSides(.south, s);
            if (sides.get(.east)) |s| region.sides += try self.countSides(.east, s);
            if (sides.get(.west)) |s| region.sides += try self.countSides(.west, s);

            if (region.area != 0) {
                try self.regions.append(region);
            }
        }
    }

    fn totalPrice(self: *const @This(), comptime priceType: PriceType) Int {
        var sum: Int = 0;
        for (self.regions.items) |r| {
            sum += r.price(priceType);
        }
        return sum;
    }
};

fn part1(input: []const u8, ally: std.mem.Allocator) !Int {
    var map = try Map.init(input, ally);
    defer map.deinit();
    try map.walk();
    return map.totalPrice(.perimeter);
}

fn part2(input: []const u8, ally: std.mem.Allocator) !i32 {
    var map = try Map.init(input, ally);
    defer map.deinit();
    try map.walk();
    return map.totalPrice(.side);
}

test "part 1,2" {
    const example1 =
        \\AAAA
        \\BBCD
        \\BBCC
        \\EEEC
    ;
    const example2 =
        \\OOOOO
        \\OXOXO
        \\OOOOO
        \\OXOXO
        \\OOOOO
    ;
    const example3 =
        \\EEEEE
        \\EXXXX
        \\EEEEE
        \\EXXXX
        \\EEEEE
    ;
    const example4 =
        \\AAAAAA
        \\AAABBA
        \\AAABBA
        \\ABBAAA
        \\ABBAAA
        \\AAAAAA
    ;
    const example5 =
        \\RRRRIICCFF
        \\RRRRIICCCF
        \\VVRRRCCFFF
        \\VVRCCCJFFF
        \\VVVVCJJCFE
        \\VVIVCCJJEE
        \\VVIIICJJEE
        \\MIIIIIJJEE
        \\MIIISIJEEE
        \\MMMISSJEEE
    ;

    try std.testing.expectEqual(140, try part1(example1, std.testing.allocator));
    try std.testing.expectEqual(772, try part1(example2, std.testing.allocator));
    try std.testing.expectEqual(692, try part1(example3, std.testing.allocator));
    try std.testing.expectEqual(1184, try part1(example4, std.testing.allocator));
    try std.testing.expectEqual(1930, try part1(example5, std.testing.allocator));

    try std.testing.expectEqual(80, try part2(example1, std.testing.allocator));
    try std.testing.expectEqual(436, try part2(example2, std.testing.allocator));
    try std.testing.expectEqual(236, try part2(example3, std.testing.allocator));
    try std.testing.expectEqual(368, try part2(example4, std.testing.allocator));
    try std.testing.expectEqual(1206, try part2(example5, std.testing.allocator));
}
