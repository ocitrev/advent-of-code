const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/2024/day/23
    utils.printTitle(2024, 23, "LAN Party");

    const m = utils.Monitor.init();
    defer m.deinit();
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const ally = gpa.allocator();
    const input = comptime utils.trim_input(@embedFile("input"));

    const p1 = try part1(ally, input);
    utils.printAnswer(1, p1);
    std.debug.assert(1000 == p1);

    const p2 = try part2(ally, input);
    defer ally.free(p2);
    utils.printAnswer(2, p2);
    std.debug.assert(std.mem.eql(u8, "cf,ct,cv,cz,fi,lq,my,pa,sl,tt,vw,wz,yd", p2));
}

const Int = i32;
const String = []const u8;

const Graph = struct {
    edges: std.StringHashMap(std.StringHashMap(void)),

    fn init(ally: std.mem.Allocator, input: []const u8) !Graph {
        var edges = std.StringHashMap(std.StringHashMap(void)).init(ally);
        var lineIt = std.mem.tokenizeAny(u8, input, "\r\n");
        while (lineIt.next()) |line| {
            var it = std.mem.splitScalar(u8, line, '-');
            const a = it.next().?;
            const b = it.next().?;

            if (edges.getPtr(a)) |e| {
                try e.*.put(b, {});
            } else {
                var e = std.StringHashMap(void).init(ally);
                try e.put(b, {});
                try edges.put(a, e);
            }

            if (edges.getPtr(b)) |e| {
                try e.*.put(a, {});
            } else {
                var e = std.StringHashMap(void).init(ally);
                try e.put(a, {});
                try edges.put(b, e);
            }
        }

        return .{
            .edges = edges,
        };
    }

    fn deinit(self: *@This()) void {
        var it = self.edges.valueIterator();
        while (it.next()) |v| {
            v.*.deinit();
        }
        self.edges.deinit();
    }
};

fn part1(ally: std.mem.Allocator, input: []const u8) !Int {
    var graph = try Graph.init(ally, input);
    defer graph.deinit();

    var computers = std.ArrayList(String).init(ally);
    defer computers.deinit();

    var keyIt = graph.edges.keyIterator();
    while (keyIt.next()) |key| {
        try computers.append(key.*);
    }
    std.mem.sort(String, computers.items, {}, stringLess);

    var sum: Int = 0;
    var itA = graph.edges.keyIterator();

    while (itA.next()) |aa| {
        const a = aa.*;
        var itB = graph.edges.iterator();

        while (itB.next()) |entryB| {
            const b = entryB.key_ptr.*;
            if (std.mem.order(u8, a, b) != .lt) {
                continue;
            }

            const setB = entryB.value_ptr;
            var itC = graph.edges.iterator();

            while (itC.next()) |entryC| {
                const c = entryC.key_ptr.*;
                if (std.mem.order(u8, b, c) != .lt) {
                    continue;
                }

                const setC = entryC.value_ptr;

                if (setB.*.contains(a) and
                    setC.*.contains(a) and
                    setC.*.contains(b))
                {
                    if (std.mem.startsWith(u8, a, "t") or
                        std.mem.startsWith(u8, b, "t") or
                        std.mem.startsWith(u8, c, "t"))
                    {
                        sum += 1;
                    }
                }
            }
        }
    }

    return sum;
}

fn stringLess(_: void, a: String, b: String) bool {
    return std.mem.order(u8, a, b) == .lt;
}

fn part2(ally: std.mem.Allocator, input: []const u8) !String {
    var graph = try Graph.init(ally, input);
    defer graph.deinit();

    var computers = std.ArrayList(String).init(ally);
    defer computers.deinit();

    var keyIt = graph.edges.keyIterator();
    while (keyIt.next()) |key| {
        try computers.append(key.*);
    }

    const seed: u64 = @intCast(std.time.nanoTimestamp());
    var rng = std.Random.DefaultPrng.init(seed);
    var clique = std.ArrayList(String).init(ally);
    defer clique.deinit();

    var best: ?String = null;
    var bestLen: ?usize = null;

    for (0..1_000) |_| {
        clique.clearRetainingCapacity();
        std.rand.shuffle(rng.random(), String, computers.items);

        outer: for (computers.items) |x| {
            for (clique.items) |y| {
                if (!graph.edges.getPtr(y).?.*.contains(x)) {
                    continue :outer;
                }
            }

            try clique.append(x);
        }

        if (bestLen == null or clique.items.len > bestLen.?) {
            if (best != null) ally.free(best.?);
            std.mem.sort(String, clique.items, {}, stringLess);
            best = try std.mem.join(ally, ",", clique.items);
            bestLen = clique.items.len;
        }
    }

    return best.?;
}

test "parts 1,2" {
    const ally = std.testing.allocator;
    const example =
        \\kh-tc
        \\qp-kh
        \\de-cg
        \\ka-co
        \\yn-aq
        \\qp-ub
        \\cg-tb
        \\vc-aq
        \\tb-ka
        \\wh-tc
        \\yn-cg
        \\kh-ub
        \\ta-co
        \\de-co
        \\tc-td
        \\tb-wq
        \\wh-td
        \\ta-ka
        \\td-qp
        \\aq-cg
        \\wq-ub
        \\ub-vc
        \\de-ta
        \\wq-aq
        \\wq-vc
        \\wh-yn
        \\ka-de
        \\kh-ta
        \\co-tc
        \\wh-qp
        \\tb-vc
        \\td-yn
    ;
    try std.testing.expectEqual(7, try part1(ally, example));

    const p2 = try part2(ally, example);
    defer ally.free(p2);
    try std.testing.expectEqualStrings("co,de,ka,ta", p2);
}
