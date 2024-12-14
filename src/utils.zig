const std = @import("std");

pub fn Result(comptime T: type) type {
    return struct {
        part1: T = 0,
        part2: T = 0,

        pub fn init() @This() {
            return .{};
        }
    };
}

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

    pub fn nextSequence(self: *@This(), delimiter: []const u8) ?[]const u8 {
        var it = std.mem.tokenizeSequence(u8, self.input, delimiter);
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

        pub fn add(self: *const @This(), x: T, y: T) @This() {
            return .{ .x = self.x + x, .y = self.y + y };
        }

        pub fn addp(self: *const @This(), rhs: @This()) @This() {
            return .{ .x = self.x + rhs.x, .y = self.y + rhs.y };
        }

        pub fn sub(self: *const @This(), x: T, y: T) @This() {
            return .{ .x = self.x - x, .y = self.y - y };
        }

        pub fn subp(self: *const @This(), rhs: @This()) @This() {
            return .{ .x = self.x - rhs.x, .y = self.y - rhs.y };
        }

        pub fn mul(self: *const @This(), rhs: T) @This() {
            return .{ .x = self.x * rhs, .y = self.y * rhs };
        }

        pub fn rotate90Left(self: *const @This()) @This() {
            return .{ .x = self.y, .y = -self.x };
        }

        pub fn rotate90Right(self: *const @This()) @This() {
            return .{ .x = -self.y, .y = self.x };
        }

        pub fn wrap(self: *const @This(), width: T, height: T) @This() {
            return .{
                .x = @mod(self.x, width),
                .y = @mod(self.y, height),
            };
        }

        pub fn format(self: *const @This(), comptime fmt: []const u8, options: std.fmt.FormatOptions, writer: anytype) !void {
            _ = fmt;
            _ = options;
            try writer.print("({}, {})", .{ self.x, self.y });
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
    map: std.AutoHashMap(Point2d(i32), u8),
    width: i32 = 0,
    height: i32 = 0,

    pub fn parse(input: []const u8, allocator: std.mem.Allocator) !Grid {
        var it = std.mem.tokenizeAny(u8, input, "\r\n");
        var map = std.AutoHashMap(Point2d(i32), u8).init(allocator);
        var y: i32 = 0;
        var w: i32 = 0;

        while (it.next()) |line| {
            var x: i32 = 0;
            for (line) |c| {
                const p = Point2d(i32){ .x = x, .y = y };
                try map.put(p, c);
                x += 1;
                w = @max(w, x);
            }
            y += 1;
        }

        return Grid{ .map = map, .width = w, .height = y };
    }

    pub fn contains(self: *const @This(), p: Point2d(i32)) bool {
        if (p.x < 0 or p.y < 0) {
            return false;
        }

        if (p.x >= self.width or p.y >= self.height) {
            return false;
        }

        return true;
    }

    pub fn deinit(self: *@This()) void {
        self.map.deinit();
    }
};

pub fn trim_input(input: []const u8) []const u8 {
    return std.mem.trimRight(u8, input, " \r\n\t");
}

pub const Monitor = struct {
    start: std.time.Instant,

    pub fn init() Monitor {
        return .{ .start = std.time.Instant.now() catch unreachable };
    }

    pub fn deinit(self: *const @This()) void {
        const end = std.time.Instant.now() catch unreachable;
        const elapsed: f64 = @floatFromInt(end.since(self.start));

        if (elapsed > std.time.ns_per_s) {
            std.debug.print("  Elapsed: {d:.2}s\n", .{elapsed / std.time.ns_per_s});
        } else if (elapsed > std.time.ns_per_ms) {
            std.debug.print("  Elapsed: {d:.2}ms\n", .{elapsed / std.time.ns_per_ms});
        } else {
            std.debug.print("  Elapsed: {d:.2}µs\n", .{elapsed / std.time.ns_per_us});
        }
    }
};

pub fn HashMapArray(comptime K: type, comptime V: type) type {
    const HashMapType = std.AutoHashMap(K, std.ArrayList(V));
    return struct {
        data: HashMapType,
        ally: std.mem.Allocator,

        pub fn init(ally: std.mem.Allocator) @This() {
            return @This(){
                .data = HashMapType.init(ally),
                .ally = ally,
            };
        }

        pub fn deinit(self: *@This()) void {
            var it = self.data.iterator();
            while (it.next()) |entry| {
                entry.value_ptr.*.deinit();
            }
            self.data.deinit();
        }

        pub fn put(self: *@This(), key: K, value: V) !void {
            if (self.data.getPtr(key)) |list| {
                try list.*.append(value);
            } else {
                var newList = std.ArrayList(V).init(self.ally);
                try newList.append(value);
                try self.data.put(key, newList);
            }
        }

        pub fn get(self: *const @This(), key: K) ?[]V {
            if (self.data.get(key)) |list| {
                return list.items;
            }
            return null;
        }
    };
}

pub fn reduce(comptime T: type, slice: []const T, initial: T, func: fn (T, T) T) T {
    var result = initial;
    for (slice) |v| {
        result = func(result, v);
    }
    return result;
}

pub fn mul(comptime T: type) fn (T, T) T {
    return struct {
        fn call(a: T, v: T) T {
            return a * v;
        }
    }.call;
}

pub fn add(comptime T: type) fn (T, T) T {
    return struct {
        fn call(a: T, v: T) T {
            return a + v;
        }
    }.call;
}
