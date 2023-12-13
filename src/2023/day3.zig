const std = @import("std");
const Point2d = @import("utils").Point2d(i32);

fn isSymbol(c: u8) bool {
    return c != '.' and (c < '0' or c > '9');
}

test "isSymbol" {
    inline for ("'*/+=@-&%$") |c| {
        try std.testing.expectEqual(true, comptime isSymbol(c));
    }
    inline for (".123456789") |c| {
        try std.testing.expectEqual(false, comptime isSymbol(c));
    }
}

const Schematic = struct {
    map: std.AutoHashMap(Point2d, u8),
    partNumberMap: std.AutoHashMap(Point2d, usize),
    partNumbers: std.ArrayList(i32),
    w: i32 = 0,
    h: i32 = 0,

    pub fn parse(input: []const u8, allocator: std.mem.Allocator) !Schematic {
        var result = Schematic{
            .map = std.AutoHashMap(Point2d, u8).init(allocator),
            .partNumberMap = std.AutoHashMap(Point2d, usize).init(allocator),
            .partNumbers = std.ArrayList(i32).init(allocator),
        };

        var y: i32 = 0;
        var it = std.mem.tokenizeAny(u8, input, "\r\n");

        while (it.next()) |line| {
            var x: i32 = 0;

            for (line) |c| {
                const p = Point2d{ .x = x, .y = y };
                try result.map.put(p, c);
                x += 1;
                result.w = @max(result.w, x);
            }

            y += 1;
        }

        result.h = y + 1;
        try result.parsePartNumbers(allocator);
        return result;
    }

    fn isPartNumber(self: *Schematic, p: Point2d, len: i8) bool {
        for ([_]i8{ -1, 0, 1 }) |y| {
            const firstX = p.x - len;
            const lastX = p.x + 1;

            var x: i32 = firstX;
            while (x <= lastX) {
                if (y == 0 and x > firstX and x < lastX) {
                    x += 1;
                    continue;
                }
                const nei = Point2d{ .x = x, .y = p.y + y };
                var c = self.map.get(nei) orelse '.';
                if (isSymbol(c)) {
                    return true;
                }

                x += 1;
            }
        }

        return false;
    }

    fn parsePartNumbers(self: *Schematic, allocator: std.mem.Allocator) !void {
        var lastNumber: ?Point2d = null;
        var numLen: i8 = 0;
        var num: i32 = 0;
        var numberCoords = std.ArrayList(Point2d).init(allocator);
        defer numberCoords.deinit();

        const h = @as(usize, @intCast(self.h));
        const w = @as(usize, @intCast(self.w));
        for (0..h) |yy| {
            const y = @as(i32, @intCast(yy));
            for (0..w) |xx| {
                const x = @as(i32, @intCast(xx));
                var p = Point2d{ .x = x, .y = y };

                if (self.map.get(p)) |c| {
                    if (c >= '0' and c <= '9') {
                        num *= 10;
                        num += c - '0';
                        numLen += 1;
                        lastNumber = p;
                        try numberCoords.append(p);
                    } else if (lastNumber) |ln| {
                        if (self.isPartNumber(ln, numLen)) {
                            const index = self.partNumbers.items.len;
                            for (numberCoords.items) |pt| {
                                try self.partNumberMap.put(pt, index);
                            }
                            try self.partNumbers.append(num);
                        }

                        numberCoords.clearAndFree();
                        num = 0;
                        numLen = 0;
                        lastNumber = null;
                    }
                }
            }
        }
    }

    pub fn getTotal(self: *const Schematic) i32 {
        var total: i32 = 0;
        for (self.partNumbers.items) |num| {
            total += num;
        }
        return total;
    }

    fn getGearRatios(self: *const Schematic) !i32 {
        var total: i32 = 0;
        var y: i32 = 0;
        while (y != self.h) : (y += 1) {
            var x: i32 = 0;
            while (x != self.w) : (x += 1) {
                const p = Point2d{ .x = x, .y = y };
                const c = self.map.get(p) orelse '.';

                if (c != '*')
                    continue;

                var a: ?usize = null;
                var b: ?usize = null;
                var yy: i32 = -1;

                while (yy <= 1) : (yy += 1) {
                    var xx: i32 = -1;
                    while (xx <= 1) : (xx += 1) {
                        if (xx == 0 and yy == 0)
                            continue;

                        const pp = Point2d{ .x = p.x + xx, .y = p.y + yy };

                        if (self.partNumberMap.get(pp)) |v| {
                            if (a == null) {
                                a = v;
                            } else if (b == null) {
                                if (a != v) {
                                    b = v;
                                }
                            } else if (b != v) {
                                unreachable;
                            }
                        }
                    }
                }

                if (a != null and b != null) {
                    total += self.partNumbers.items[a.?] * self.partNumbers.items[b.?];
                }
            }
        }

        return total;
    }

    pub fn deinit(self: *Schematic) void {
        self.map.deinit();
        self.partNumbers.deinit();
        self.partNumberMap.deinit();
    }
};

fn part1(input: []const u8, allocator: std.mem.Allocator) !i32 {
    var schematic = try Schematic.parse(input, allocator);
    defer schematic.deinit();
    return schematic.getTotal();
}

test "part 1" {
    const example =
        \\467..114..
        \\...*......
        \\..35..633.
        \\......#...
        \\617*......
        \\.....+.58.
        \\..592.....
        \\......755.
        \\...$.*....
        \\.664.598..
    ;
    try std.testing.expectEqual(@as(i32, 4361), try part1(example, std.testing.allocator));
}

fn part2(input: []const u8, allocator: std.mem.Allocator) !i32 {
    var schematic = try Schematic.parse(input, allocator);
    defer schematic.deinit();
    return schematic.getGearRatios();
}

test "part 2" {
    const example =
        \\467..114..
        \\...*......
        \\..35..633.
        \\......#...
        \\617*......
        \\.....+.58.
        \\..592.....
        \\......755.
        \\...$.*....
        \\.664.598..
    ;
    try std.testing.expectEqual(@as(i32, 467835), try part2(example, std.testing.allocator));
}

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();
    const input = @embedFile("input");

    // https://adventofcode.com/2023/day/3
    std.debug.print("Day 3, 2023: Gear Ratios\n", .{});
    std.debug.print("  Part 1: {}\n", .{try part1(input, allocator)});
    std.debug.print("  Part 2: {}\n", .{try part2(input, allocator)});
}
