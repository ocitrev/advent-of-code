const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/2025/day/5
    utils.printTitle(2025, 5, "Cafeteria");

    const m = utils.Monitor.init();
    defer m.deinit();
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const ally = gpa.allocator();
    const input = comptime utils.trimInput(@embedFile("input"));

    const p1 = try part1(ally, input);
    utils.printAnswer(1, p1);
    std.debug.assert(888 == p1);

    const p2 = try part2(ally, input);
    utils.printAnswer(2, p2);
    std.debug.assert(344378119285354 == p2);
}

const Int = u64;

const Range = struct {
    begin: Int,
    end: Int,
};

fn sortRanges(_: void, a: Range, b: Range) bool {
    return a.begin < b.begin;
}

const Cafeteria = struct {
    fresh: std.ArrayList(Range),
    ingredients: std.ArrayList(Int),
    ally: std.mem.Allocator,

    pub fn parse(ally: std.mem.Allocator, input: []const u8) !@This() {
        var fresh = std.ArrayList(Range).empty;
        var ingredients = std.ArrayList(Int).empty;

        var parsingRanges = true;
        var lineIt = std.mem.splitAny(u8, input, "\r\n");
        while (lineIt.next()) |line| {
            if (parsingRanges) {
                if (line.len == 0) {
                    parsingRanges = false;
                    continue;
                }

                var rangeIt = std.mem.splitScalar(u8, line, '-');
                const range = Range{
                    .begin = std.fmt.parseInt(Int, rangeIt.next().?, 10) catch unreachable,
                    .end = std.fmt.parseInt(Int, rangeIt.next().?, 10) catch unreachable,
                };
                try fresh.append(ally, range);
            } else {
                const ingredient = std.fmt.parseInt(Int, line, 10) catch unreachable;
                try ingredients.append(ally, ingredient);
            }
        }

        var result = Cafeteria{
            .fresh = fresh,
            .ingredients = ingredients,
            .ally = ally,
        };
        try result.mergeRanges();
        return result;
    }

    pub fn deinit(self: *@This()) void {
        self.fresh.deinit(self.ally);
        self.ingredients.deinit(self.ally);
    }

    pub fn isFresh(self: *const @This(), num: Int) bool {
        for (self.fresh.items) |r| {
            if (num >= r.begin and num <= r.end) {
                return true;
            }
        }

        return false;
    }

    pub fn countFreshIngredientsFromList(self: *const @This()) Int {
        var total: Int = 0;
        for (self.ingredients.items) |ing| {
            if (self.isFresh(ing)) {
                total += 1;
            }
        }
        return total;
    }

    pub fn mergeRanges(self: *@This()) !void {
        std.mem.sort(Range, self.fresh.items, {}, sortRanges);
        var mergedRanges = std.ArrayList(Range).empty;
        try mergedRanges.append(self.ally, self.fresh.items[0]);

        for (self.fresh.items) |r| {
            const lastIndex = mergedRanges.items.len - 1;
            var lastRange = &mergedRanges.items[lastIndex];
            if (r.begin <= lastRange.end + 1) {
                if (r.end > lastRange.end) {
                    lastRange.end = r.end;
                }
            } else {
                try mergedRanges.append(self.ally, r);
            }
        }

        self.fresh.deinit(self.ally);
        self.fresh = mergedRanges;
    }

    pub fn totalFreshIngredients(self: *const @This()) Int {
        var total: Int = 0;
        for (self.fresh.items) |r| {
            total += (r.end - r.begin + 1);
        }

        return total;
    }
};

fn part1(ally: std.mem.Allocator, input: []const u8) !Int {
    var cafeteria = try Cafeteria.parse(ally, input);
    defer cafeteria.deinit();
    return cafeteria.countFreshIngredientsFromList();
}

fn part2(ally: std.mem.Allocator, input: []const u8) !Int {
    var cafeteria = try Cafeteria.parse(ally, input);
    defer cafeteria.deinit();
    return cafeteria.totalFreshIngredients();
}

test "parts 1,2" {
    const ally = std.testing.allocator;
    const example =
        \\3-5
        \\10-14
        \\16-20
        \\12-18
        \\
        \\1
        \\5
        \\8
        \\11
        \\17
        \\32
    ;
    try std.testing.expectEqual(3, try part1(ally, example));
    try std.testing.expectEqual(14, try part2(ally, example));
}
