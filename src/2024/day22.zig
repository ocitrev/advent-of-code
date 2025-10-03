const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/2024/day/22
    utils.printTitle(2024, 22, "Monkey Market");

    const m = utils.Monitor.init();
    defer m.deinit();
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const ally = gpa.allocator();
    const input = comptime utils.trimInput(@embedFile("input"));

    const p1 = try part1(input);
    utils.printAnswer(1, p1);
    std.debug.assert(13004408787 == p1);

    const p2 = try part2(ally, input);
    utils.printAnswer(2, p2);
    std.debug.assert(1455 == p2);
}

const Int = u64;

const Rng = struct {
    value: Int,

    pub fn init(seed: Int) Rng {
        return .{ .value = seed };
    }

    fn next(self: *@This()) Int {
        self.value = @mod(self.value ^ (self.value << 6), 16777216);
        self.value = @mod(self.value ^ (self.value >> 5), 16777216);
        self.value = @mod(self.value ^ (self.value << 11), 16777216);

        return self.value;
    }

    fn run(self: *@This(), comptime n: usize) Int {
        for (0..n) |_| {
            _ = self.next();
        }
        return self.value;
    }
};

fn part1(input: []const u8) !Int {
    var sum: Int = 0;
    var lineIt = std.mem.tokenizeAny(u8, input, "\r\n");
    while (lineIt.next()) |line| {
        const n = try std.fmt.parseInt(Int, line, 10);
        var rng = Rng.init(n);
        sum += rng.run(2000);
    }

    return sum;
}

fn part2(ally: std.mem.Allocator, input: []const u8) !Int {
    var prices = std.ArrayList(i8).empty;
    defer prices.deinit(ally);

    var changes = std.ArrayList(i8).empty;
    defer changes.deinit(ally);

    var maxPrices = std.AutoHashMap([4]i8, Int).init(ally);
    defer maxPrices.deinit();

    var bananas = std.AutoHashMap([4]i8, Int).init(ally);
    defer bananas.deinit();

    var lineIt = std.mem.tokenizeAny(u8, input, "\r\n");
    while (lineIt.next()) |line| {
        prices.clearRetainingCapacity();
        changes.clearRetainingCapacity();
        bananas.clearRetainingCapacity();

        const n = try std.fmt.parseInt(Int, line, 10);
        var rng = Rng.init(n);
        var prev: i8 = @intCast(@rem(n, 10));
        try prices.append(ally, prev);

        for (0..2000) |_| {
            const next = rng.next();
            const price: i8 = @intCast(@rem(next, 10));
            try prices.append(ally, price);
            try changes.append(ally, price - prev);
            prev = price;
        }

        var changesIt = std.mem.window(i8, changes.items, 4, 1);
        var pricesIt = std.mem.window(i8, prices.items[1..], 4, 1);

        while (changesIt.next()) |w| {
            const k = [4]i8{ w[0], w[1], w[2], w[3] };
            const p = pricesIt.next().?[3];
            if (!bananas.contains(k)) {
                try bananas.putNoClobber(k, @intCast(p));
            }
        }

        var it = bananas.iterator();
        while (it.next()) |entry| {
            const k = entry.key_ptr.*;
            const v = entry.value_ptr.*;

            if (maxPrices.getPtr(k)) |max| {
                max.* += v;
            } else {
                try maxPrices.put(k, v);
            }
        }
    }

    var max: Int = 0;
    var it = maxPrices.valueIterator();
    while (it.next()) |v| {
        max = @max(max, v.*);
    }

    return max;
}

test "123" {
    comptime {
        var rng = Rng.init(123);
        try std.testing.expectEqual(15887950, rng.next());
        try std.testing.expectEqual(16495136, rng.next());
        try std.testing.expectEqual(527345, rng.next());
        try std.testing.expectEqual(704524, rng.next());
        try std.testing.expectEqual(1553684, rng.next());
        try std.testing.expectEqual(12683156, rng.next());
        try std.testing.expectEqual(11100544, rng.next());
        try std.testing.expectEqual(12249484, rng.next());
        try std.testing.expectEqual(7753432, rng.next());
        try std.testing.expectEqual(5908254, rng.next());
    }
}

test "parts 1" {
    const example =
        \\1
        \\10
        \\100
        \\2024
    ;

    @setEvalBranchQuota(100000);
    comptime try std.testing.expectEqual(37327623, try part1(example));
}

test "part 2" {
    const ally = std.testing.allocator;
    const example =
        \\1
        \\2
        \\3
        \\2024
    ;

    try std.testing.expectEqual(23, try part2(ally, example));
}
