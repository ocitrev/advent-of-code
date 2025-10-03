const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/2024/day/21
    utils.printTitle(2024, 21, "Keypad Conundrum");

    const m = utils.Monitor.init();
    defer m.deinit();
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const ally = gpa.allocator();
    const input = comptime utils.trimInput(@embedFile("input"));

    const p1 = try part1(ally, input);
    utils.printAnswer(1, p1);
    std.debug.assert(270084 == p1);

    const p2 = try part2(ally, input);
    utils.printAnswer(2, p2);
    std.debug.assert(329431019997766 == p2);
}

const Int = i64;
const String = []const u8;
const Point2d = utils.Point2d(Int);

const Node = struct {
    coord: Point2d,
    dir: Point2d,
    cost: Int,
};

const StringBuilder = struct {
    string: std.ArrayList(u8),
    ally: std.mem.Allocator,

    fn init(ally: std.mem.Allocator) !StringBuilder {
        return .{
            .string = std.ArrayList(u8).empty,
            .ally = ally,
        };
    }

    fn deinit(self: *@This()) void {
        self.string.deinit(self.ally);
    }

    fn push(self: *@This(), c: u8) !void {
        try self.string.append(self.ally, c);
    }

    fn pushNTimes(self: *@This(), c: u8, n: usize) !void {
        try self.string.appendNTimes(self.ally, c, n);
    }

    fn toOwnedSlice(self: *@This()) ![]const u8 {
        return try self.string.toOwnedSlice(self.ally);
    }
};

fn nodeOrder(_: void, a: *const Node, b: *const Node) std.math.Order {
    return std.math.order(a.cost, b.cost);
}

const Keypad = struct {
    keys: []const String,
    start: Point2d,

    fn getCoord(self: *const @This(), key: u8) Point2d {
        for (0.., self.keys) |y, row| {
            for (0.., row) |x, k| {
                if (k == key) {
                    return .{ .x = @intCast(x), .y = @intCast(y) };
                }
            }
        }

        std.debug.panic("Invalid key: {}", .{key});
    }

    fn getPaths(self: *const @This(), ally: std.mem.Allocator, from: u8, to: u8) ![]String {
        var result = std.ArrayList(String).empty;
        defer result.deinit(ally);

        var builder = try StringBuilder.init(ally);
        defer builder.deinit();

        if (from == to) {
            try builder.push('A');
            try result.append(ally, try builder.toOwnedSlice());
            return try result.toOwnedSlice(ally);
        }

        const avoid = self.getCoord(' ');
        const fromCoord = self.getCoord(from);
        const toCoord = self.getCoord(to);
        const dir = toCoord.subp(fromCoord);

        const vertical: u8 = if (dir.y < 0) '^' else 'v';
        const horizontal: u8 = if (dir.x < 0) '<' else '>';
        const nbX: usize = @intCast(@abs(dir.x));
        const nbY: usize = @intCast(@abs(dir.y));

        if (nbX == 0) {
            try builder.pushNTimes(vertical, nbY);
            try builder.push('A');
            try result.append(ally, try builder.toOwnedSlice());
            return try result.toOwnedSlice(ally);
        }

        if (nbY == 0) {
            try builder.pushNTimes(horizontal, nbX);
            try builder.push('A');
            try result.append(ally, try builder.toOwnedSlice());
            return try result.toOwnedSlice(ally);
        }

        const moveVerticalFirst = fromCoord.y == avoid.y and toCoord.x == avoid.x;
        const moveHorizontalFirst = fromCoord.x == avoid.x and toCoord.y == avoid.y;

        if (moveVerticalFirst) {
            try builder.pushNTimes(vertical, nbY);
            try builder.pushNTimes(horizontal, nbX);
            try builder.push('A');
            try result.append(ally, try builder.toOwnedSlice());
        } else if (moveHorizontalFirst) {
            try builder.pushNTimes(horizontal, nbX);
            try builder.pushNTimes(vertical, nbY);
            try builder.push('A');
            try result.append(ally, try builder.toOwnedSlice());
        } else {
            try builder.pushNTimes(horizontal, nbX);
            try builder.pushNTimes(vertical, nbY);
            try builder.push('A');
            try result.append(ally, try builder.toOwnedSlice());
            try builder.pushNTimes(vertical, nbY);
            try builder.pushNTimes(horizontal, nbX);
            try builder.push('A');
            try result.append(ally, try builder.toOwnedSlice());
        }

        return try result.toOwnedSlice(ally);
    }
};

// Numeric keypad:
// +---+---+---+
// | 7 | 8 | 9 |
// +---+---+---+
// | 4 | 5 | 6 |
// +---+---+---+
// | 1 | 2 | 3 |
// +---+---+---+
//     | 0 | A |
//     +---+---+
const NUMERIC_KEY_PAD = Keypad{
    .keys = &.{
        "789",
        "456",
        "123",
        " 0A",
    },
    .start = Point2d.init(2, 3),
};

// Directional keypad:
//     +---+---+
//     | ^ | A |
// +---+---+---+
// | < | v | > |
// +---+---+---+
const DIRECTIONAL_KEY_PAD = Keypad{
    .keys = &.{
        " ^A",
        "<v>",
    },
    .start = .{ .x = 2, .y = 0 },
};

const Cache = std.AutoHashMap(u64, Int);

pub inline fn cacheKey(combo: []const u8, depth: usize) u64 {
    var buffer: [20]u8 = undefined;
    const text = std.fmt.bufPrint(&buffer, "{}@{s}", .{ depth, combo }) catch unreachable;
    return std.hash.Murmur2_64.hash(text);
}

fn getPathLen(
    ally: std.mem.Allocator,
    keypad: *const Keypad,
    input: String,
    depth: usize,
    cache: *Cache,
) !Int {
    if (depth == 0) {
        return @intCast(input.len);
    }

    const k = cacheKey(input, depth);

    if (cache.get(k)) |len| {
        return len;
    }

    var total: Int = 0;
    var prev: u8 = 'A';

    for (input) |c| {
        const paths = try keypad.getPaths(ally, prev, c);
        defer {
            for (paths) |p| {
                ally.free(p);
            }
            ally.free(paths);
        }

        var best: ?Int = null;

        for (paths) |p| {
            const len = try getPathLen(ally, &DIRECTIONAL_KEY_PAD, p, depth - 1, cache);
            if (best == null or len < best.?) {
                best = len;
            }
        }

        prev = c;
        total += best.?;
    }

    try cache.put(k, total);
    return total;
}

fn part1(ally: std.mem.Allocator, input: String) !Int {
    var cache = Cache.init(ally);
    defer cache.deinit();
    var combos = std.ArrayList(String).empty;
    defer combos.deinit(ally);

    var lineIt = std.mem.tokenizeAny(u8, input, "\r\n");
    while (lineIt.next()) |line| {
        try combos.append(ally, line);
    }

    var total: Int = 0;
    for (combos.items) |combo| {
        const num = try std.fmt.parseInt(Int, combo[0 .. combo.len - 1], 10);
        const len = try getPathLen(ally, &NUMERIC_KEY_PAD, combo, 3, &cache);
        total += len * num;
    }

    return total;
}

fn part2(ally: std.mem.Allocator, input: String) !Int {
    var cache = Cache.init(ally);
    defer cache.deinit();

    var combos = std.ArrayList(String).empty;
    defer combos.deinit(ally);

    var lineIt = std.mem.tokenizeAny(u8, input, "\r\n");
    while (lineIt.next()) |line| {
        try combos.append(ally, line);
    }

    var total: Int = 0;
    for (combos.items) |combo| {
        const num = try std.fmt.parseInt(Int, combo[0 .. combo.len - 1], 10);
        const len = try getPathLen(ally, &NUMERIC_KEY_PAD, combo, 26, &cache);
        total += len * num;
    }

    return total;
}

fn getNbPaths(ally: std.mem.Allocator, expectedPaths: []const String, from: u8, to: u8) !void {
    const paths = try NUMERIC_KEY_PAD.getPaths(ally, from, to);
    defer {
        for (paths) |p| {
            ally.free(p);
        }
        ally.free(paths);
    }

    try std.testing.expectEqual(expectedPaths.len, paths.len);
    for (expectedPaths, paths) |expected, p| {
        try std.testing.expectEqualStrings(expected, p);
    }
}

test "parts 1,2" {
    const ally = std.testing.allocator;
    const example =
        \\029A
        \\980A
        \\179A
        \\456A
        \\379A
    ;
    try std.testing.expectEqual(126384, try part1(ally, example));
    try std.testing.expectEqual(154115708116294, try part2(ally, example));
}
