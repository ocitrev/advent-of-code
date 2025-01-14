const std = @import("std");
const utils = @import("utils");
const Point2d = utils.Point2d(i16);

pub fn main() !void {
    // https://adventofcode.com/2024/day/4
    utils.printTitle(2024, 4, "Ceres Search");

    const m = utils.Monitor.init();
    defer m.deinit();
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const ally = gpa.allocator();
    const input = comptime utils.trimInput(@embedFile("input"));

    const p1 = try part1(input, ally);
    utils.printAnswer(1, p1);
    std.debug.assert(2530 == p1);

    const p2 = try part2(input, ally);
    utils.printAnswer(2, p2);
    std.debug.assert(1921 == p2);
}

const Map = struct {
    map: std.AutoHashMap(Point2d, u8),
    width: usize,
    height: usize,

    pub fn deinit(self: *@This()) void {
        self.map.deinit();
    }

    pub fn print(self: *@This()) void {
        std.debug.print("map\n", .{});
        for (0..self.height) |y| {
            for (0..self.width) |x| {
                const p = Point2d{ .x = @intCast(x), .y = @intCast(y) };
                std.debug.print("{c}", .{self.map.get(p) orelse '.'});
            }
            std.debug.print("\n", .{});
        }
    }
};

fn parseMap(input: []const u8, ally: std.mem.Allocator) !Map {
    var map = std.AutoHashMap(Point2d, u8).init(ally);
    var it = std.mem.tokenizeAny(u8, input, "\r\n");
    var width: usize = 0;

    var y: usize = 0;
    while (it.next()) |line| : (y += 1) {
        for (0.., line) |x, c| {
            width = @max(x, width);
            switch (c) {
                'X', 'M', 'A', 'S', '0'...'9' => {
                    const p = Point2d{ .x = @intCast(x), .y = @intCast(y) };
                    try map.put(p, c);
                },
                else => {},
            }
        }
    }
    return .{ .map = map, .width = width + 1, .height = y };
}

fn countXmas(input: []const u8) i32 {
    var count: i32 = 0;
    const needle = "XMAS";
    var it = std.mem.window(u8, input, needle.len, 1);
    while (it.next()) |text| {
        if (std.mem.eql(u8, text, needle)) {
            count += 1;
        }
    }

    return count;
}

fn part1(input: []const u8, ally: std.mem.Allocator) !i32 {
    var map = try parseMap(input, ally);
    defer map.deinit();
    const needle = "MAS";
    var buffer: [needle.len]u8 = undefined;

    var sum: i32 = 0;
    for (0..map.height) |y| {
        for (0..map.width) |x| {
            const p = Point2d{ .x = @intCast(x), .y = @intCast(y) };
            if (map.map.get(p) == 'X') {
                buffer[0] = map.map.get(p.add(1, 0)) orelse '.';
                buffer[1] = map.map.get(p.add(2, 0)) orelse '.';
                buffer[2] = map.map.get(p.add(3, 0)) orelse '.';
                if (std.mem.eql(u8, &buffer, needle)) sum += 1;

                buffer[0] = map.map.get(p.add(0, 1)) orelse '.';
                buffer[1] = map.map.get(p.add(0, 2)) orelse '.';
                buffer[2] = map.map.get(p.add(0, 3)) orelse '.';
                if (std.mem.eql(u8, &buffer, needle)) sum += 1;

                buffer[0] = map.map.get(p.add(-1, 0)) orelse '.';
                buffer[1] = map.map.get(p.add(-2, 0)) orelse '.';
                buffer[2] = map.map.get(p.add(-3, 0)) orelse '.';
                if (std.mem.eql(u8, &buffer, needle)) sum += 1;

                buffer[0] = map.map.get(p.add(0, -1)) orelse '.';
                buffer[1] = map.map.get(p.add(0, -2)) orelse '.';
                buffer[2] = map.map.get(p.add(0, -3)) orelse '.';
                if (std.mem.eql(u8, &buffer, needle)) sum += 1;

                buffer[0] = map.map.get(p.add(1, 1)) orelse '.';
                buffer[1] = map.map.get(p.add(2, 2)) orelse '.';
                buffer[2] = map.map.get(p.add(3, 3)) orelse '.';
                if (std.mem.eql(u8, &buffer, needle)) sum += 1;

                buffer[0] = map.map.get(p.add(-1, 1)) orelse '.';
                buffer[1] = map.map.get(p.add(-2, 2)) orelse '.';
                buffer[2] = map.map.get(p.add(-3, 3)) orelse '.';
                if (std.mem.eql(u8, &buffer, needle)) sum += 1;

                buffer[0] = map.map.get(p.add(1, -1)) orelse '.';
                buffer[1] = map.map.get(p.add(2, -2)) orelse '.';
                buffer[2] = map.map.get(p.add(3, -3)) orelse '.';
                if (std.mem.eql(u8, &buffer, needle)) sum += 1;

                buffer[0] = map.map.get(p.add(-1, -1)) orelse '.';
                buffer[1] = map.map.get(p.add(-2, -2)) orelse '.';
                buffer[2] = map.map.get(p.add(-3, -3)) orelse '.';
                if (std.mem.eql(u8, &buffer, needle)) sum += 1;
            }
        }
    }

    return sum;
}

test "part 1" {
    const example =
        \\MMMSXXMASM
        \\MSAMXMSMSA
        \\AMXSXMAAMM
        \\MSAMASMSMX
        \\XMASAMXAMM
        \\XXAMMXXAMA
        \\SMSMSASXSS
        \\SAXAMASAAA
        \\MAMMMXMMMM
        \\MXMXAXMASX
    ;
    try std.testing.expectEqual(18, try part1(example, std.testing.allocator));
}

fn part2(input: []const u8, ally: std.mem.Allocator) !i32 {
    var map = try parseMap(input, ally);
    defer map.deinit();

    var sum: i32 = 0;
    for (0..map.height) |y| {
        for (0..map.width) |x| {
            const p = Point2d{ .x = @intCast(x), .y = @intCast(y) };
            if (map.map.get(p) == 'A') {
                const nw = map.map.get(p.north().west());
                const ne = map.map.get(p.north().east());
                const sw = map.map.get(p.south().west());
                const se = map.map.get(p.south().east());

                if (nw != 'M' and nw != 'S') continue;
                if (ne != 'M' and ne != 'S') continue;
                if (sw != 'M' and sw != 'S') continue;
                if (se != 'M' and se != 'S') continue;
                if (nw == se or ne == sw) continue;

                sum += 1;
            }
        }
    }

    return sum;
}

test "part 2" {
    const example =
        \\MMMSXXMASM
        \\MSAMXMSMSA
        \\AMXSXMAAMM
        \\MSAMASMSMX
        \\XMASAMXAMM
        \\XXAMMXXAMA
        \\SMSMSASXSS
        \\SAXAMASAAA
        \\MAMMMXMMMM
        \\MXMXAXMASX
    ;
    try std.testing.expectEqual(9, try part2(example, std.testing.allocator));
}
