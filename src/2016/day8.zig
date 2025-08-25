const std = @import("std");
const utils = @import("utils");
const Point2d = utils.Point2d(i32);

pub fn main() !void {
    // https://adventofcode.com/2016/day/8
    utils.printTitle(2016, 8, "Two-Factor Authentication");

    const m = utils.Monitor.init();
    defer m.deinit();

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();
    const input = comptime @embedFile("input");

    var s = Screen.init(50, 6, allocator);
    defer s.deinit();
    s.run(input);

    utils.printAnswer(1, s.countLit());
    utils.printAnswer(2, "");
    try s.print("  ");
}

const Screen = struct {
    data: std.AutoHashMap(Point2d, void),
    width: i32,
    height: i32,

    pub fn init(w: i32, h: i32, allocator: std.mem.Allocator) Screen {
        const map = std.AutoHashMap(Point2d, void).init(allocator);
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
                self.data.put(pt, {}) catch unreachable;
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
        std.debug.assert(buffer.len >= self.width);

        var x: i32 = 0;
        while (x != self.width) : (x += 1) {
            const pt = Point2d{ .x = x, .y = row };
            buffer[@intCast(x)] = self.data.contains(pt);
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

        var buffer: [6]bool = undefined;
        std.debug.assert(buffer.len >= self.height);

        var y: i32 = 0;
        while (y != self.height) : (y += 1) {
            const pt = Point2d{ .x = column, .y = y };
            buffer[@intCast(y)] = self.data.contains(pt);
        }
        y = 0;
        while (y != self.height) : (y += 1) {
            const pt = Point2d{ .x = column, .y = y + count };
            self.setWrapping(pt, buffer[@intCast(y)]);
        }
    }

    fn setWrapping(self: *@This(), pt: Point2d, v: bool) void {
        const wrapped = pt.wrap(self.width, self.height);
        if (v) {
            self.data.put(wrapped, {}) catch unreachable;
        } else {
            _ = self.data.remove(wrapped);
        }
    }

    fn getChar(self: *const @This(), pt: Point2d) []const u8 {
        return if (self.data.contains(pt)) "\u{2588}" else " ";
    }

    pub fn print(self: *const @This(), indent: []const u8) !void {
        var y: i32 = 0;

        var stdout_buffer: [1024]u8 = undefined;
        var stdout_writer = std.fs.File.stdout().writer(&stdout_buffer);
        const stdout = &stdout_writer.interface;

        while (y != self.height) : (y += 1) {
            try stdout.print("{s}", .{indent});
            var x: i32 = 0;
            while (x != self.width) : (x += 1) {
                const pt = Point2d{ .x = x, .y = y };
                try stdout.print("{s}", .{self.getChar(pt)});
            }

            try stdout.print("\n", .{});
        }

        try stdout.flush();
    }

    pub fn countLit(self: *@This()) i32 {
        return @intCast(self.data.count());
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
