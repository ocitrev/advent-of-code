const std = @import("std");
const clipboard = @import("clipboard.zig");

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
    const Vec = @Vector(2, T);
    return struct {
        x: T = 0,
        y: T = 0,

        pub const ZERO = @This(){};
        pub const NORTH = @This().init(0, -1);
        pub const SOUTH = @This().init(0, 1);
        pub const EAST = @This().init(1, 0);
        pub const WEST = @This().init(-1, 0);

        pub inline fn fromVec(v: Vec) @This() {
            return .{ .x = v[0], .y = v[1] };
        }

        pub inline fn vec(self: *const @This()) Vec {
            return .{ self.x, self.y };
        }

        pub inline fn init(x: T, y: T) @This() {
            return .{ .x = x, .y = y };
        }

        pub fn north(self: *const @This()) @This() {
            return fromVec(self.vec() + NORTH.vec());
        }
        pub fn south(self: *const @This()) @This() {
            return fromVec(self.vec() + SOUTH.vec());
        }
        pub fn west(self: *const @This()) @This() {
            return fromVec(self.vec() + WEST.vec());
        }
        pub fn east(self: *const @This()) @This() {
            return fromVec(self.vec() + EAST.vec());
        }

        pub fn eql(a: @This(), b: @This()) bool {
            return a.x == b.x and a.y == b.y;
        }

        pub fn add(self: *const @This(), x: T, y: T) @This() {
            return fromVec(self.vec() + Vec{ x, y });
        }

        pub fn addp(self: *const @This(), rhs: @This()) @This() {
            return fromVec(self.vec() + rhs.vec());
        }

        pub fn sub(self: *const @This(), x: T, y: T) @This() {
            return fromVec(self.vec() - Vec{ x, y });
        }

        pub fn subp(self: *const @This(), rhs: @This()) @This() {
            return fromVec(self.vec() - rhs.vec());
        }

        pub fn mul(self: *const @This(), rhs: T) @This() {
            return fromVec(self.vec() * @as(Vec, @splat(rhs)));
        }

        pub fn neg(self: *const @This()) @This() {
            return .{ .x = -self.x, .y = -self.y };
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

        pub fn manhatanDistance(self: *const @This(), rhs: @This()) T {
            const x = Vec{ self.x, rhs.x };
            const y = Vec{ self.y, rhs.y };
            return @reduce(.Max, x) - @reduce(.Min, x) + @reduce(.Max, y) - @reduce(.Min, y);
        }

        pub fn format(
            self: *const @This(),
            comptime fmt: []const u8,
            options: std.fmt.FormatOptions,
            writer: anytype,
        ) !void {
            _ = fmt;
            _ = options;
            try writer.print("({}, {})", .{ self.vec[0], self.vec[1] });
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
        const ansi = std.io.getStdErr().getOrEnableAnsiEscapeSupport();
        const color = if (elapsed < std.time.ns_per_ms * 100)
            "\x1b[32m"
        else if (elapsed < std.time.ns_per_ms * 500)
            "\x1b[33m"
        else
            "\x1b[31m";

        const prefix = if (ansi) color else "";
        const suffix = if (ansi) "\x1b[m" else "";
        var units: []const u8 = undefined;
        var value: f64 = undefined;

        if (elapsed > std.time.ns_per_s) {
            units = "s";
            value = elapsed / std.time.ns_per_s;
        } else if (elapsed > std.time.ns_per_ms) {
            units = "ms";
            value = elapsed / std.time.ns_per_ms;
        } else {
            units = "µs";
            value = elapsed / std.time.ns_per_us;
        }

        std.debug.print("  Elapsed: {s}{d:.2}{s}{s}\n", .{ prefix, value, units, suffix });
    }
};

pub fn HashMapArray(comptime K: type, comptime V: type) type {
    const HashMapType = if (K == []const u8)
        std.StringHashMap(std.ArrayList(V))
    else
        std.AutoHashMap(K, std.ArrayList(V));

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
                entry.value_ptr.deinit();
            }
            self.data.deinit();
        }

        pub fn put(self: *@This(), key: K, value: V) !void {
            if (self.data.getPtr(key)) |list| {
                try list.append(value);
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

        pub fn iterator(self: *const @This()) @TypeOf(self.data.iterator()) {
            return self.data.iterator();
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

fn setClipboardResult(comptime format: []const u8, number: anytype) !void {
    var buffer: [64]u8 = undefined;
    const textResult = try std.fmt.bufPrint(&buffer, format, .{number});
    try clipboard.setClipboardText(textResult, .private);
}

fn isEmptyInt(comptime T: type, value: T) bool {
    return value == 0;
}

fn isEmptyString(comptime _: type, text: []const u8) bool {
    return text.len == 0;
}

pub fn printTitle(comptime year: u16, comptime day: u8, comptime title: []const u8) void {
    const w = std.io.getStdOut().writer();
    w.print("Day {}, {}: {s}\n", .{ year, day, title }) catch unreachable;
}

fn isString(T: type) bool {
    return T == []const u8 or T == []u8;
}

pub fn printAnswer(comptime part: u2, result: anytype) void {
    const T = @TypeOf(result);
    const format = if (comptime isString(T)) "{s}" else "{}";
    const isEmtpy = if (comptime isString(T)) isEmptyString else isEmptyInt;

    const w = std.io.getStdOut().writer();
    w.print("  Part {}: " ++ format ++ "\n", .{ part, result }) catch unreachable;

    if (!isEmtpy(T, result)) {
        setClipboardResult(format, result) catch |err| {
            std.debug.print("Warning: Clipboard failed: {}\n", .{err});
        };
    }
}
