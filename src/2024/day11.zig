const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/2024/day/11
    utils.printTitle(2024, 11, "Plutonian Pebbles");

    const m = utils.Monitor.init();
    defer m.deinit();
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const ally = gpa.allocator();
    const input = comptime utils.trim_input(@embedFile("input"));

    const p1 = part1(input, ally);
    utils.printAnswer(1, p1);
    std.debug.assert(183484 == p1);

    const p2 = part2(input, ally);
    utils.printAnswer(2, p2);
    std.debug.assert(218817038947400 == p2);
}

const Int = u64;
const Cache = std.AutoHashMap(struct { Int, Int }, Int);

fn nbDigits(num: Int) Int {
    return std.math.log10_int(num) + 1;
}

fn blink(stone: Int, depth: Int, cache: *Cache) Int {
    if (depth == 0) {
        return 1;
    }

    const key = .{ stone, depth };
    if (cache.get(key)) |result| {
        return result;
    }

    if (stone == 0) {
        const result = blink(1, depth - 1, cache);
        cache.put(key, result) catch unreachable;
        return result;
    }

    const digits = nbDigits(stone);
    if (digits & 1 == 0) {
        const p = std.math.powi(Int, 10, @divExact(digits, 2)) catch unreachable;
        var result = blink(@divTrunc(stone, p), depth - 1, cache);
        result += blink(@rem(stone, p), depth - 1, cache);
        cache.put(key, result) catch unreachable;
        return result;
    }

    const result = blink(stone * 2024, depth - 1, cache);
    cache.put(key, result) catch unreachable;
    return result;
}

fn run(input: []const u8, depth: Int, ally: std.mem.Allocator) Int {
    var cache = Cache.init(ally);
    defer cache.deinit();

    var sum: Int = 0;
    var it = std.mem.tokenizeScalar(u8, input, ' ');

    while (it.next()) |numText| {
        const stone = std.fmt.parseInt(Int, numText, 10) catch unreachable;
        sum += blink(stone, depth, &cache);
    }

    return sum;
}

test run {
    const example =
        \\125 17
    ;
    try std.testing.expectEqual(22, run(example, 6, std.testing.allocator));
    try std.testing.expectEqual(55312, run(example, 25, std.testing.allocator));
    try std.testing.expectEqual(65601038650482, run(example, 75, std.testing.allocator));
}

fn part1(input: []const u8, ally: std.mem.Allocator) Int {
    return run(input, 25, ally);
}

fn part2(input: []const u8, ally: std.mem.Allocator) Int {
    return run(input, 75, ally);
}
