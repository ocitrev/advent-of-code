const std = @import("std");
const Point2d = @import("utils").Point2d(i32);

const Grid = struct {
    map: std.AutoHashMap(Point2d, u8),
    width: i32 = 0,
    height: i32 = 0,
    start: Point2d,
    nbDots: u32 = 0,

    pub fn parse(input: []const u8, allocator: std.mem.Allocator) !Grid {
        var map = std.AutoHashMap(Point2d, u8).init(allocator);
        var start = Point2d{};
        var y: i32 = 0;
        var w: i32 = 0;
        var nbDots: u32 = 0;
        var it = std.mem.tokenizeAny(u8, input, "\r\n");
        while (it.next()) |line| {
            var x: i32 = 0;
            for (line) |c| {
                const p = Point2d{ .x = x, .y = y };
                if (c == 'S') {
                    start = p;
                } else if (c == '.') {
                    nbDots += 1;
                }

                try map.put(p, c);
                x += 1;
                w = @max(w, x);
            }

            y += 1;
        }

        return Grid{ .map = map, .start = start, .width = w, .height = y, .nbDots = nbDots };
    }

    pub fn deinit(self: *@This()) void {
        self.map.deinit();
    }

    pub fn replaceInvalidTiles(self: *@This(), allocator: std.mem.Allocator) !void {
        var current = self.getStart();
        try self.map.put(current.p, current.c);
        var valid = std.AutoHashMap(Point2d, void).init(allocator);
        defer valid.deinit();

        while (true) {
            try valid.put(current.p, {});
            current = self.getNext(current);
            if (Point2d.eql(current.p, self.start)) {
                break;
            }
        }

        var it = self.map.iterator();
        while (it.next()) |elem| {
            if (elem.value_ptr.* != '.' and valid.get(elem.key_ptr.*) == null) {
                elem.value_ptr.* = '.';
            }
        }
    }

    fn validH(west: u8, east: u8) bool {
        return switch (west) {
            'F', 'L', '-' => east == '7' or east == 'J' or east == '-',
            else => false,
        };
    }

    fn validV(north: u8, south: u8) bool {
        return switch (north) {
            'F', '7', '|' => south == 'L' or south == 'J' or south == '|',
            else => false,
        };
    }

    fn get(self: *const @This(), p: Point2d) u8 {
        if (self.map.get(p)) |c| {
            return c;
        }
        return ' ';
    }

    const Dir = enum {
        invalid,
        west,
        north,
        east,
        south,
    };

    const Tile = struct {
        c: u8,
        p: Point2d,
        from: Dir,
    };

    pub fn getStart(self: *const @This()) Tile {
        const west = self.get(self.start.west());
        const north = self.get(self.start.north());
        const east = self.get(self.start.east());
        const south = self.get(self.start.south());
        var start: u8 = 0;
        var from = Dir.invalid;

        if (Grid.validV(north, '|') and Grid.validV('|', south)) {
            start = '|';
            from = Dir.north;
        } else if (Grid.validH(west, '-') and Grid.validH('-', east)) {
            start = '-';
            from = Dir.east;
        } else if (Grid.validV('F', south) and Grid.validH('F', east)) {
            start = 'F';
            from = Dir.north;
        } else if (Grid.validV('7', south) and Grid.validH(west, '7')) {
            start = '7';
            from = Dir.east;
        } else if (Grid.validV(north, 'L') and Grid.validH('L', east)) {
            start = 'L';
            from = Dir.south;
        } else if (Grid.validV(north, 'J') and Grid.validH(west, 'J')) {
            start = 'J';
            from = Dir.east;
        }

        return Tile{ .c = start, .p = self.start, .from = from };
    }

    pub fn getNext(self: *const @This(), tile: Tile) Tile {
        const dir = switch (tile.from) {
            .east => switch (tile.c) {
                '-' => Dir.east,
                '7' => Dir.south,
                'J' => Dir.north,
                else => Dir.invalid,
            },
            .north => switch (tile.c) {
                'F' => Dir.east,
                '7' => Dir.west,
                '|' => Dir.north,
                else => Dir.invalid,
            },
            .west => switch (tile.c) {
                'F' => Dir.south,
                'L' => Dir.north,
                '-' => Dir.west,
                else => Dir.invalid,
            },
            .south => switch (tile.c) {
                'L' => Dir.east,
                'J' => Dir.west,
                '|' => Dir.south,
                else => Dir.invalid,
            },
            .invalid => Dir.invalid,
        };

        return switch (dir) {
            .west => Tile{ .c = self.get(tile.p.west()), .p = tile.p.west(), .from = dir },
            .east => Tile{ .c = self.get(tile.p.east()), .p = tile.p.east(), .from = dir },
            .north => Tile{ .c = self.get(tile.p.north()), .p = tile.p.north(), .from = dir },
            .south => Tile{ .c = self.get(tile.p.south()), .p = tile.p.south(), .from = dir },
            else => Tile{ .c = self.get(tile.p), .p = tile.p, .from = dir },
        };
    }

    pub fn countLoop(self: *const @This()) u32 {
        var current = self.getStart();
        var count: u32 = 0;

        while (true) {
            count += 1;
            current = self.getNext(current);

            if (Point2d.eql(current.p, self.start)) {
                break;
            }
        }

        return count / 2;
    }

    fn isVert(west: u8, east: u8) bool {
        if (west == '-' or east == '-')
            return false;

        if ((west == 'F' or west == 'L') and (east == '7' or east == 'J'))
            return false;

        return true;
    }

    fn isHorz(north: u8, south: u8) bool {
        if (north == '|' or south == '|')
            return false;

        if ((north == 'J' or north == 'L') and (south == '7' or south == 'F'))
            return false;

        return true;
    }

    pub fn countTiles(self: *const @This(), allocator: std.mem.Allocator) !u32 {
        var insideSet = std.AutoHashMap(Point2d, void).init(allocator);
        defer insideSet.deinit();

        var y: i32 = 0;
        while (y != self.height) : (y += 1) {
            var outside: bool = true;
            var x: i32 = 0;
            while (x != self.width) : (x += 1) {
                const p = Point2d{ .x = x, .y = y };
                const c = self.get(p);

                outside = switch (c) {
                    '|', 'L', 'J' => !outside,
                    else => outside,
                };

                if (c == '.' and !outside) {
                    try insideSet.put(p, {});
                }
            }
        }

        const print = false;
        if (comptime print) {
            y = 0;
            while (y != self.height) : (y += 1) {
                var x: i32 = 0;
                while (x != self.width) : (x += 1) {
                    const p = Point2d{ .x = x, .y = y };

                    if (insideSet.get(p)) |_| {
                        std.debug.print("\x1b[31mI\x1b[m", .{});
                    } else {
                        std.debug.print("{c}", .{self.get(p)});
                    }
                }
                std.debug.print("\n", .{});
            }
            std.debug.print("\n", .{});
        }

        return insideSet.count();
    }
};

test "horizontal connections" {
    try std.testing.expect(comptime !Grid.validH('F', 'F'));
    try std.testing.expect(comptime Grid.validH('F', '7'));
    try std.testing.expect(comptime Grid.validH('F', 'J'));
    try std.testing.expect(comptime !Grid.validH('F', 'L'));
    try std.testing.expect(comptime Grid.validH('F', '-'));
    try std.testing.expect(comptime !Grid.validH('F', '|'));
    try std.testing.expect(comptime !Grid.validH('7', 'F'));
    try std.testing.expect(comptime !Grid.validH('7', '7'));
    try std.testing.expect(comptime !Grid.validH('7', 'J'));
    try std.testing.expect(comptime !Grid.validH('7', 'L'));
    try std.testing.expect(comptime !Grid.validH('7', '-'));
    try std.testing.expect(comptime !Grid.validH('7', '|'));
    try std.testing.expect(comptime !Grid.validH('J', 'F'));
    try std.testing.expect(comptime !Grid.validH('J', '7'));
    try std.testing.expect(comptime !Grid.validH('J', 'J'));
    try std.testing.expect(comptime !Grid.validH('J', 'L'));
    try std.testing.expect(comptime !Grid.validH('J', '-'));
    try std.testing.expect(comptime !Grid.validH('J', '|'));
    try std.testing.expect(comptime !Grid.validH('L', 'F'));
    try std.testing.expect(comptime Grid.validH('L', '7'));
    try std.testing.expect(comptime Grid.validH('L', 'J'));
    try std.testing.expect(comptime !Grid.validH('L', 'L'));
    try std.testing.expect(comptime Grid.validH('L', '-'));
    try std.testing.expect(comptime !Grid.validH('L', '|'));
    try std.testing.expect(comptime !Grid.validH('-', 'F'));
    try std.testing.expect(comptime Grid.validH('-', '7'));
    try std.testing.expect(comptime Grid.validH('-', 'J'));
    try std.testing.expect(comptime !Grid.validH('-', 'L'));
    try std.testing.expect(comptime Grid.validH('-', '-'));
    try std.testing.expect(comptime !Grid.validH('-', '|'));
    try std.testing.expect(comptime !Grid.validH('|', 'F'));
    try std.testing.expect(comptime !Grid.validH('|', '7'));
    try std.testing.expect(comptime !Grid.validH('|', 'J'));
    try std.testing.expect(comptime !Grid.validH('|', 'L'));
    try std.testing.expect(comptime !Grid.validH('|', '-'));
    try std.testing.expect(comptime !Grid.validH('|', '|'));
}

test "vertical connections" {
    try std.testing.expect(comptime !Grid.validV('F', 'F'));
    try std.testing.expect(comptime !Grid.validV('F', '7'));
    try std.testing.expect(comptime Grid.validV('F', 'J'));
    try std.testing.expect(comptime Grid.validV('F', 'L'));
    try std.testing.expect(comptime !Grid.validV('F', '-'));
    try std.testing.expect(comptime Grid.validV('F', '|'));
    try std.testing.expect(comptime !Grid.validV('7', 'F'));
    try std.testing.expect(comptime !Grid.validV('7', '7'));
    try std.testing.expect(comptime Grid.validV('7', 'J'));
    try std.testing.expect(comptime Grid.validV('7', 'L'));
    try std.testing.expect(comptime !Grid.validV('7', '-'));
    try std.testing.expect(comptime Grid.validV('7', '|'));
    try std.testing.expect(comptime !Grid.validV('J', 'F'));
    try std.testing.expect(comptime !Grid.validV('J', '7'));
    try std.testing.expect(comptime !Grid.validV('J', 'J'));
    try std.testing.expect(comptime !Grid.validV('J', 'L'));
    try std.testing.expect(comptime !Grid.validV('J', '-'));
    try std.testing.expect(comptime !Grid.validV('J', '|'));
    try std.testing.expect(comptime !Grid.validV('L', 'F'));
    try std.testing.expect(comptime !Grid.validV('L', '7'));
    try std.testing.expect(comptime !Grid.validV('L', 'J'));
    try std.testing.expect(comptime !Grid.validV('L', 'L'));
    try std.testing.expect(comptime !Grid.validV('L', '-'));
    try std.testing.expect(comptime !Grid.validV('L', '|'));
    try std.testing.expect(comptime !Grid.validV('-', 'F'));
    try std.testing.expect(comptime !Grid.validV('-', '7'));
    try std.testing.expect(comptime !Grid.validV('-', 'J'));
    try std.testing.expect(comptime !Grid.validV('-', 'L'));
    try std.testing.expect(comptime !Grid.validV('-', '-'));
    try std.testing.expect(comptime !Grid.validV('-', '|'));
    try std.testing.expect(comptime !Grid.validV('|', 'F'));
    try std.testing.expect(comptime !Grid.validV('|', '7'));
    try std.testing.expect(comptime Grid.validV('|', 'J'));
    try std.testing.expect(comptime Grid.validV('|', 'L'));
    try std.testing.expect(comptime !Grid.validV('|', '-'));
    try std.testing.expect(comptime Grid.validV('|', '|'));
}

fn part1(input: []const u8, allocator: std.mem.Allocator) !u32 {
    var grid = try Grid.parse(input, allocator);
    defer grid.deinit();
    return grid.countLoop();
}

test "part 1" {
    const example1 =
        \\.....
        \\.S-7.
        \\.|.|.
        \\.L-J.
        \\.....
    ;
    try std.testing.expectEqual(@as(u32, 4), try part1(example1, std.testing.allocator));

    const example2 =
        \\..F7.
        \\.FJ|.
        \\SJ.L7
        \\|F--J
        \\LJ...
    ;
    try std.testing.expectEqual(@as(u32, 8), try part1(example2, std.testing.allocator));
}

fn part2(input: []const u8, allocator: std.mem.Allocator) !u32 {
    var grid = try Grid.parse(input, allocator);
    defer grid.deinit();
    try grid.replaceInvalidTiles(allocator);
    return try grid.countTiles(allocator);
}

test "part 2.1" {
    const example =
        \\...........
        \\.S-------7.
        \\.|F-----7|.
        \\.||.....||.
        \\.||.....||.
        \\.|L-7.F-J|.
        \\.|..|.|..|.
        \\.L--J.L--J.
        \\...........
    ;
    try std.testing.expectEqual(@as(u32, 4), try part2(example, std.testing.allocator));
}

test "part 2.2" {
    const example =
        \\..........
        \\.S------7.
        \\.|F----7|.
        \\.||....||.
        \\.||....||.
        \\.|L-7F-J|.
        \\.|..||..|.
        \\.L--JL--J.
        \\..........
    ;
    try std.testing.expectEqual(@as(u32, 4), try part2(example, std.testing.allocator));
}

test "part 2.3" {
    const example =
        \\.F----7F7F7F7F-7....
        \\.|F--7||||||||FJ....
        \\.||.FJ||||||||L7....
        \\FJL7L7LJLJ||LJ.L-7..
        \\L--J.L7...LJS7F-7L7.
        \\....F-J..F7FJ|L7L7L7
        \\....L7.F7||L7|.L7L7|
        \\.....|FJLJ|FJ|F7|.LJ
        \\....FJL-7.||.||||...
        \\....L---J.LJ.LJLJ...
    ;
    try std.testing.expectEqual(@as(u32, 8), try part2(example, std.testing.allocator));
}

test "part 2.4" {
    const example =
        \\FF7FSF7F7F7F7F7F---7
        \\L|LJ||||||||||||F--J
        \\FL-7LJLJ||||||LJL-77
        \\F--JF--7||LJLJ7F7FJ-
        \\L---JF-JLJ.||-FJLJJ7
        \\|F|F-JF---7F7-L7L|7|
        \\|FFJF7L7F-JF7|JL---7
        \\7-L-JL7||F7|L7F-7F7|
        \\L.L7LFJ|||||FJL7||LJ
        \\L7JLJL-JLJLJL--JLJ.L
    ;
    try std.testing.expectEqual(@as(u32, 10), try part2(example, std.testing.allocator));
}

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();

    // https://adventofcode.com/2023/day/10
    const input = @embedFile("input");
    std.debug.print("Day 10, 2023: Pipe Maze\n", .{});
    std.debug.print("  Part 1: {}\n", .{try part1(input, allocator)});
    std.debug.print("  Part 2: {}\n", .{try part2(input, allocator)});
}
