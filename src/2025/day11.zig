const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/2025/day/11
    utils.printTitle(2025, 11, "Reactor");

    const m = utils.Monitor.init();
    defer m.deinit();
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const ally = gpa.allocator();
    const input = comptime utils.trimInput(@embedFile("input"));

    const p1 = try part1(ally, input);
    utils.printAnswer(1, p1);
    std.debug.assert(571 == p1);

    const p2 = try part2(ally, input);
    utils.printAnswer(2, p2);
    std.debug.assert(511378159390560 == p2);
}

const Int = u64;

const Graph = struct {
    nodes: utils.HashMapArray([]const u8, []const u8),
    ally: std.mem.Allocator,

    fn parse(ally: std.mem.Allocator, input: []const u8) !Graph {
        var nodes = utils.HashMapArray([]const u8, []const u8).init(ally);
        var lineIt = std.mem.tokenizeAny(u8, input, "\r\n");

        while (lineIt.next()) |line| {
            var it = std.mem.tokenizeAny(u8, line, " :");
            const key = it.next().?;
            while (it.next()) |v| {
                try nodes.put(key, v);
            }
        }

        return Graph{
            .nodes = nodes,
            .ally = ally,
        };
    }

    fn deinit(self: *@This()) void {
        self.nodes.deinit();
    }

    const K = struct { u64, ?bool, ?bool };

    fn countPaths(
        self: *const @This(),
        from: []const u8,
        seenFft: bool,
        seenDac: bool,
        cache: *std.AutoHashMap(K, Int),
    ) !Int {
        if (std.mem.eql(u8, from, "out")) {
            return if (seenFft and seenDac) 1 else 0;
        }

        var localFft = seenFft;
        if (!localFft and std.mem.eql(u8, from, "fft")) {
            localFft = true;
        }

        var localDac = seenDac;
        if (!localDac and std.mem.eql(u8, from, "dac")) {
            localDac = true;
        }

        const k = .{ std.hash.Murmur2_64.hash(from), localFft, localDac };
        if (cache.get(k)) |cached| {
            return cached;
        }

        var total: Int = 0;
        if (self.nodes.get(from)) |nodes| {
            for (nodes) |n| {
                total += try self.countPaths(n, localFft, localDac, cache);
            }
        }

        try cache.put(k, total);
        return total;
    }

    fn countYouPaths(self: *const @This()) !Int {
        var cache = std.AutoHashMap(K, Int).init(self.ally);
        defer cache.deinit();
        return try self.countPaths("you", true, true, &cache);
    }

    fn countSvrPaths(self: *const @This()) !Int {
        var cache = std.AutoHashMap(K, Int).init(self.ally);
        defer cache.deinit();
        return try self.countPaths("svr", false, false, &cache);
    }
};

fn part1(ally: std.mem.Allocator, input: []const u8) !Int {
    var g = try Graph.parse(ally, input);
    defer g.deinit();
    return try g.countYouPaths();
}

fn part2(ally: std.mem.Allocator, input: []const u8) !Int {
    var g = try Graph.parse(ally, input);
    defer g.deinit();
    return try g.countSvrPaths();
}

test "parts 1,2" {
    const ally = std.testing.allocator;
    const example1 =
        \\aaa: you hhh
        \\you: bbb ccc
        \\bbb: ddd eee
        \\ccc: ddd eee fff
        \\ddd: ggg
        \\eee: out
        \\fff: out
        \\ggg: out
        \\hhh: ccc fff iii
        \\iii: out
    ;
    try std.testing.expectEqual(5, try part1(ally, example1));

    const example2 =
        \\svr: aaa bbb
        \\aaa: fft
        \\fft: ccc
        \\bbb: tty
        \\tty: ccc
        \\ccc: ddd eee
        \\ddd: hub
        \\hub: fff
        \\eee: dac
        \\dac: fff
        \\fff: ggg hhh
        \\ggg: out
        \\hhh: out
    ;
    try std.testing.expectEqual(2, try part2(ally, example2));
}
