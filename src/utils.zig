const std = @import("std");

pub const Parser = struct {
    input: []const u8,

    pub fn init(input: []const u8) Parser {
        return Parser{ .input = input };
    }

    pub fn nextScalar(self: *@This(), delimiter: u8) ?[]const u8 {
        var it = std.mem.tokenizeScalar(u8, self.input, delimiter);
        if (it.next()) |result| {
            self.input = it.rest();
            return result;
        }
        return null;
    }

    pub fn nextAny(self: *@This(), delimiter: []const u8) ?[]const u8 {
        var it = std.mem.tokenizeAny(u8, self.input, delimiter);
        if (it.next()) |result| {
            self.input = it.rest();
            return result;
        }
        return null;
    }

    pub fn rest(self: *@This()) []const u8 {
        return self.input;
    }
};

pub fn Point2d(comptime T: type) type {
    return struct {
        x: T = 0,
        y: T = 0,

        pub fn north(self: *const @This()) @This() {
            return .{ .x = self.x, .y = self.y - 1 };
        }
        pub fn south(self: *const @This()) @This() {
            return .{ .x = self.x, .y = self.y + 1 };
        }
        pub fn west(self: *const @This()) @This() {
            return .{ .x = self.x - 1, .y = self.y };
        }
        pub fn east(self: *const @This()) @This() {
            return .{ .x = self.x + 1, .y = self.y };
        }

        pub fn eql(a: @This(), b: @This()) bool {
            return a.x == b.x and a.y == b.y;
        }
    };
}

pub fn Point3d(comptime T: type) type {
    return struct {
        x: T = 0,
        y: T = 0,
        z: T = 0,
    };
}

pub const Grid = struct {
    map: std.AutoHashMap(Point2d, u8),
    width: i32 = 0,
    height: i32 = 0,

    pub fn parseFromLineIterator(it: anytype, allocator: std.mem.Allocator) !Grid {
        var map = std.AutoHashMap(Point2d, u8).init(allocator);
        var y: i32 = 0;
        var w: i32 = 0;

        while (it.next()) |line| {
            var x: i32 = 0;
            for (line) |c| {
                const p = Point2d{ .x = x, .y = y };
                try map.put(p, c);
                x += 1;
                w = @max(w, x);
            }
            y += 1;
        }

        return Grid{ .map = map, .width = w, .height = y };
    }

    pub fn parse(input: []const u8, allocator: std.mem.Allocator) !Grid {
        return parseFromLineIterator(std.mem.tokenizeAny(u8, input, "\r\n"), allocator);
    }

    pub fn deinit(self: *@This()) void {
        self.map.deinit();
    }
};

pub fn sum(comptime T: type, values: []const T) T {
    var result: T = 0;
    for (values) |v| {
        result += v;
    }
    return result;
}
