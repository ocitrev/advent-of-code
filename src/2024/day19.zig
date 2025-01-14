const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/2024/day/19
    utils.printTitle(2024, 19, "Linen Layout");

    const m = utils.Monitor.init();
    defer m.deinit();
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const ally = gpa.allocator();
    const input = comptime utils.trimInput(@embedFile("input"));

    const p1 = try part1(ally, input);
    utils.printAnswer(1, p1);
    std.debug.assert(358 == p1);

    const p2 = try part2(ally, input);
    utils.printAnswer(2, p2);
    std.debug.assert(600639829400603 == p2);
}

const Int = i64;

const Linen = struct {
    ally: std.mem.Allocator,
    towels: [][]const u8,
    designs: [][]const u8,

    fn init(ally: std.mem.Allocator, input: []const u8) !Linen {
        var towels = std.ArrayList([]const u8).init(ally);
        defer towels.deinit();
        var designs = std.ArrayList([]const u8).init(ally);
        defer designs.deinit();

        var parsingTowels = true;

        var lineIt = std.mem.splitAny(u8, input, "\r\n");
        while (lineIt.next()) |line| {
            if (parsingTowels) {
                if (line.len == 0) {
                    parsingTowels = false;
                } else {
                    var it = std.mem.tokenizeAny(u8, line, ", ");
                    while (it.next()) |towel| {
                        try towels.append(towel);
                    }
                }
            } else {
                try designs.append(line);
            }
        }

        return .{
            .ally = ally,
            .towels = try towels.toOwnedSlice(),
            .designs = try designs.toOwnedSlice(),
        };
    }

    fn deinit(self: *@This()) void {
        self.ally.free(self.towels);
        self.ally.free(self.designs);
    }

    pub fn isValidDesign(self: *@This(), design: []const u8) bool {
        if (design.len == 0) {
            return true;
        }

        for (self.towels) |towel| {
            if (towel.len > design.len) {
                continue;
            }
            if (std.mem.startsWith(u8, design, towel)) {
                if (self.isValidDesign(design[towel.len..])) {
                    return true;
                }
            }
        }

        return false;
    }

    fn countValidDesigns(self: *@This()) Int {
        var count: Int = 0;
        for (self.designs) |d| {
            if (self.isValidDesign(d)) {
                count += 1;
            }
        }
        return count;
    }

    fn countArrangements(self: *@This(), design: []const u8, cache: *std.StringHashMap(Int)) Int {
        if (design.len == 0) {
            return 1;
        }

        if (cache.get(design)) |count| {
            return count;
        }

        var count: Int = 0;
        for (self.towels) |towel| {
            if (towel.len > design.len) {
                continue;
            }
            if (std.mem.startsWith(u8, design, towel)) {
                count += self.countArrangements(design[towel.len..], cache);
            }
        }

        cache.put(design, count) catch unreachable;
        return count;
    }

    fn countAllArrangements(self: *@This()) Int {
        var cache = std.StringHashMap(Int).init(self.ally);
        defer cache.deinit();
        var count: Int = 0;
        for (self.designs) |d| {
            count += self.countArrangements(d, &cache);
            cache.clearRetainingCapacity();
        }
        return count;
    }
};

fn part1(ally: std.mem.Allocator, input: []const u8) !Int {
    var linen = try Linen.init(ally, input);
    defer linen.deinit();
    return linen.countValidDesigns();
}

fn part2(ally: std.mem.Allocator, input: []const u8) !Int {
    var linen = try Linen.init(ally, input);
    defer linen.deinit();
    return linen.countAllArrangements();
}

test "parts 1,2" {
    const ally = std.testing.allocator;
    const example =
        \\r, wr, b, g, bwu, rb, gb, br
        \\
        \\brwrr
        \\bggr
        \\gbbr
        \\rrbgbr
        \\ubwu
        \\bwurrg
        \\brgr
        \\bbrgwb
    ;
    try std.testing.expectEqual(6, try part1(ally, example));
    try std.testing.expectEqual(16, try part2(ally, example));
}
