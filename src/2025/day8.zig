const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/2025/day/8
    utils.printTitle(2025, 8, "Playground");

    const m = utils.Monitor.init();
    defer m.deinit();
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const ally = gpa.allocator();
    const input = comptime utils.trimInput(@embedFile("input"));

    const p1 = try part1(ally, input, 1000);
    utils.printAnswer(1, p1);
    std.debug.assert(80446 == p1);

    const p2 = try part2(ally, input);
    utils.printAnswer(2, p2);
    std.debug.assert(51294528 == p2);
}

const Int = i64;
const P3d = utils.Point3d(Int);

fn distanceSquared(a: P3d, b: P3d) Int {
    const dx: Int = @intCast(@abs(b.x - a.x));
    const dy: Int = @intCast(@abs(b.y - a.y));
    const dz: Int = @intCast(@abs(b.z - a.z));
    return dx * dx + dy * dy + dz * dz;
}

const UnionFind = struct {
    parents: []usize,
    sizes: []usize,
    ally: std.mem.Allocator,

    fn init(ally: std.mem.Allocator, count: usize) !UnionFind {
        var parents = try ally.alloc(usize, count);
        var sizes = try ally.alloc(usize, count);

        for (0..count) |i| {
            parents[i] = i;
            sizes[i] = 1;
        }

        return UnionFind{
            .parents = parents,
            .sizes = sizes,
            .ally = ally,
        };
    }

    fn deinit(self: *@This()) void {
        self.ally.free(self.parents);
        self.ally.free(self.sizes);
    }

    fn find(self: *@This(), item: usize) usize {
        if (item >= self.parents.len) {
            @panic("Index out of bounds in UnionFind.find");
        }

        if (self.parents[item] == item) {
            return item;
        }

        self.parents[item] = self.find(self.parents[item]);
        return self.parents[item];
    }

    fn mix(self: *@This(), a: usize, b: usize) bool {
        const rootA = self.find(a);
        const rootB = self.find(b);
        if (rootA != rootB) {
            if (self.sizes[rootA] < self.sizes[rootB]) {
                self.parents[rootA] = rootB;
                self.sizes[rootB] += self.sizes[rootA];
                self.sizes[rootA] = 0;
            } else {
                self.parents[rootB] = rootA;
                self.sizes[rootA] += self.sizes[rootB];
                self.sizes[rootB] = 0;
            }
            return true;
        }

        return false;
    }

    fn countUnique(self: *@This()) Int {
        var count: usize = 0;
        for (self.sizes) |size| {
            if (size > 0) {
                count += 1;
            }
        }
        return @intCast(count);
    }
};

const Playground = struct {
    points: std.ArrayList(P3d),
    distances: std.ArrayList(DistancePair),
    ally: std.mem.Allocator,

    fn parse(ally: std.mem.Allocator, input: []const u8) !@This() {
        var points = std.ArrayList(P3d).empty;
        var lineIt = std.mem.tokenizeAny(u8, input, "\r\n");
        while (lineIt.next()) |line| {
            var it = std.mem.splitScalar(u8, line, ',');
            const x = std.fmt.parseInt(Int, it.next().?, 10) catch unreachable;
            const y = std.fmt.parseInt(Int, it.next().?, 10) catch unreachable;
            const z = std.fmt.parseInt(Int, it.next().?, 10) catch unreachable;
            try points.append(ally, P3d{ .x = x, .y = y, .z = z });
        }

        var distances = std.ArrayList(DistancePair).empty;
        for (0.., points.items) |i, p1| {
            for (i + 1.., points.items[i + 1 ..]) |j, p2| {
                const dist = distanceSquared(p1, p2);
                try distances.append(ally, .{
                    .dist = dist,
                    .pair = .{ i, j },
                });
            }
        }

        std.mem.sort(DistancePair, distances.items, {}, cmpDistancePair);
        return .{
            .points = points,
            .distances = distances,
            .ally = ally,
        };
    }

    fn deinit(self: *@This()) void {
        self.points.deinit(self.ally);
        self.distances.deinit(self.ally);
    }

    const DistancePair = struct {
        dist: Int,
        pair: [2]usize,
    };

    fn cmpDistancePair(_: void, a: DistancePair, b: DistancePair) bool {
        return a.dist < b.dist;
    }

    fn getLargestCircuitsAfter(self: *@This(), comptime nbConnection: u32) !Int {
        var unionFind = try UnionFind.init(self.ally, self.points.items.len);
        defer unionFind.deinit();

        for (0.., self.distances.items) |i, pair| {
            if (i >= nbConnection) break;
            _ = unionFind.mix(pair.pair[0], pair.pair[1]);
        }

        var circuitSizes = try std.ArrayList(Int).initCapacity(self.ally, self.points.items.len);
        defer circuitSizes.deinit(self.ally);

        for (unionFind.sizes) |s| {
            if (s > 0) {
                circuitSizes.appendAssumeCapacity(@intCast(s));
            }
        }

        std.mem.sort(Int, circuitSizes.items, {}, comptime std.sort.desc(Int));
        return circuitSizes.items[0] * circuitSizes.items[1] * circuitSizes.items[2];
    }

    fn connectAll(self: *@This()) !Int {
        var unionFind = try UnionFind.init(self.ally, self.points.items.len);
        defer unionFind.deinit();

        var lastA: usize = 0;
        var lastB: usize = 0;

        for (self.distances.items) |pair| {
            lastA = pair.pair[0];
            lastB = pair.pair[1];
            if (unionFind.mix(lastA, lastB)) {
                if (unionFind.countUnique() == 1) {
                    break;
                }
            }
        }

        const a = self.points.items[lastA];
        const b = self.points.items[lastB];
        return a.x * b.x;
    }
};

fn part1(ally: std.mem.Allocator, input: []const u8, comptime nbConnection: u32) !Int {
    var playground = try Playground.parse(ally, input);
    defer playground.deinit();
    return try playground.getLargestCircuitsAfter(nbConnection);
}

fn part2(ally: std.mem.Allocator, input: []const u8) !Int {
    var playground = try Playground.parse(ally, input);
    defer playground.deinit();
    return try playground.connectAll();
}

test "parts 1,2" {
    const ally = std.testing.allocator;
    const example =
        \\162,817,812
        \\57,618,57
        \\906,360,560
        \\592,479,940
        \\352,342,300
        \\466,668,158
        \\542,29,236
        \\431,825,988
        \\739,650,466
        \\52,470,668
        \\216,146,977
        \\819,987,18
        \\117,168,530
        \\805,96,715
        \\346,949,466
        \\970,615,88
        \\941,993,340
        \\862,61,35
        \\984,92,344
        \\425,690,689
    ;
    try std.testing.expectEqual(40, try part1(ally, example, 10));
    try std.testing.expectEqual(25272, try part2(ally, example));
}
