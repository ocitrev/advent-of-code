const std = @import("std");
const utils = @import("utils");
const Point2d = utils.Point2d(i32);

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();

    // https://adventofcode.com/2016/day/7
    const input = @embedFile("input");
    std.debug.print("Day 7, 2016: \n", .{});

    var s = Screen.init(50, 6, allocator);
    defer s.deinit();
    s.run(input);

    std.debug.print("  Part 1: {}\n", .{s.countLit()});
    std.debug.print("  Part 2:\n", .{});
    s.print("  ");
}

const Screen = struct {
    data: std.AutoHashMap(Point2d, bool),
    width: i32,
    height: i32,

    pub fn init(w: i32, h: i32, allocator: std.mem.Allocator) Screen {
        const map = std.AutoHashMap(Point2d, bool).init(allocator);
        return Screen{ .data = map, .width = w, .height = h };
    }

    pub fn deinit(self: *@This()) void {
        self.data.deinit();
    }

    pub fn run(self: *@This(), input: []const u8) void {
        var it = std.mem.tokenizeAny(u8, input, "\r\n");
        while (it.next()) |line| {
            var p = utils.Parser.init(line);
            const firstToken = p.nextScalar(' ').?;

            if (std.mem.eql(u8, firstToken, "rect")) {
                self.rect(p.rest());
            } else if (std.mem.eql(u8, firstToken, "rotate")) {
                const rowOrColumn = p.nextScalar(' ').?;
                if (std.mem.eql(u8, rowOrColumn, "row")) {
                    self.rotateRow(p.rest());
                } else if (std.mem.eql(u8, rowOrColumn, "column")) {
                    self.rotateColumn(p.rest());
                }
            } else unreachable;
        }
    }

    fn rect(self: *@This(), input: []const u8) void {
        var p = utils.Parser.init(input);
        const w = std.fmt.parseInt(usize, p.nextScalar('x').?, 10) catch unreachable;
        const h = std.fmt.parseInt(usize, p.rest(), 10) catch unreachable;

        for (0..h) |y| {
            for (0..w) |x| {
                const pt = Point2d{ .x = @intCast(x), .y = @intCast(y) };
                self.data.put(pt, true) catch unreachable;
            }
        }
    }

    fn rotateRight(comptime T: type, items: []T, ammount: usize) []T {
        const len = items.len;
        const effectiveCount = @mod(ammount, len);
        return items[(len - effectiveCount)..] ++ items[0..(len - effectiveCount)];
    }

    fn rotateRow(self: *@This(), input: []const u8) void {
        var p = utils.Parser.init(input);
        _ = p.nextScalar('=').?;
        const row = std.fmt.parseInt(i32, p.nextScalar(' ').?, 10) catch unreachable;
        _ = p.nextScalar(' ').?;
        const count = std.fmt.parseInt(i32, p.rest(), 10) catch unreachable;

        var buffer: [50]bool = undefined;
        var x: i32 = 0;
        while (x != self.width) : (x += 1) {
            const pt = Point2d{ .x = x, .y = row };
            buffer[@intCast(x)] = self.get(pt);
        }
        x = 0;
        while (x != self.width) : (x += 1) {
            const pt = Point2d{ .x = x + count, .y = row };
            self.setWrapping(pt, buffer[@intCast(x)]);
        }
    }

    fn rotateColumn(self: *@This(), input: []const u8) void {
        var p = utils.Parser.init(input);
        _ = p.nextScalar('=').?;
        const column = std.fmt.parseInt(i32, p.nextScalar(' ').?, 10) catch unreachable;
        _ = p.nextScalar(' ').?;
        const count = std.fmt.parseInt(i32, p.rest(), 10) catch unreachable;

        var buffer: [50]bool = undefined;
        var y: i32 = 0;
        while (y != self.height) : (y += 1) {
            const pt = Point2d{ .x = column, .y = y };
            buffer[@intCast(y)] = self.get(pt);
        }
        y = 0;
        while (y != self.height) : (y += 1) {
            const pt = Point2d{ .x = column, .y = y + count };
            self.setWrapping(pt, buffer[@intCast(y)]);
        }
    }

    fn setWrapping(self: *@This(), pt: Point2d, v: bool) void {
        const fixedPt = Point2d{ .x = @mod(pt.x, self.width), .y = @mod(pt.y, self.height) };
        self.data.put(fixedPt, v) catch unreachable;
    }

    fn get(self: *const @This(), pt: Point2d) bool {
        if (self.data.get(pt)) |v| {
            return v;
        }

        return false;
    }

    fn getChar(self: *const @This(), pt: Point2d) []const u8 {
        if (self.get(pt)) {
            return "█";
        }

        return " ";
    }

    pub fn print(self: *const @This(), indent: []const u8) void {
        var y: i32 = 0;
        while (y != self.height) : (y += 1) {
            std.debug.print("{s}", .{indent});
            var x: i32 = 0;
            while (x != self.width) : (x += 1) {
                const pt = Point2d{ .x = x, .y = y };
                std.debug.print("{s}", .{self.getChar(pt)});
            }

            std.debug.print("\n", .{});
        }
    }

    pub fn countLit(self: *@This()) i32 {
        var count: i32 = 0;
        var it = self.data.valueIterator();
        while (it.next()) |c| {
            if (c.*) {
                count += 1;
            }
        }
        return count;
    }
};

test "parts 1,2" {
    const example =
        \\rect 3x2
        \\rotate column x=1 by 1
        \\rotate row y=0 by 4
        \\rotate column x=1 by 1
    ;
    var s = Screen.init(7, 3, std.testing.allocator);
    defer s.deinit();
    s.run(example);
    try std.testing.expectEqual(6, s.countLit());
}
