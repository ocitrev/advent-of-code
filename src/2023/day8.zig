const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/2023/day/8
    utils.printTitle(2023, 8, "Haunted Wasteland");

    const m = utils.Monitor.init();
    defer m.deinit();

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();

    const input = comptime utils.trimInput(@embedFile("input"));
    utils.printAnswer(1, try part1(input, allocator));
    utils.printAnswer(2, try part2(input, allocator));
}

const Node = struct {
    left: []const u8,
    right: []const u8,
};

const Document = struct {
    instructions: []const u8,
    map: std.StringHashMap(Node),

    pub fn countSteps(self: *const Document, start: []const u8, comptime predicate: *const fn ([]const u8) bool) u32 {
        var i: usize = 0;
        var count: u32 = 0;
        var next: []const u8 = start;

        while (!predicate(next)) {
            count += 1;
            const network = self.map.get(next).?;
            if (self.instructions[i] == 'L') {
                next = network.left;
            } else {
                next = network.right;
            }
            i += 1;
            i = i % self.instructions.len;
        }

        return count;
    }

    fn lcm(a: anytype, b: anytype) @TypeOf(a, b) {
        return (a * b) / std.math.gcd(a, b);
    }

    fn endsWithZ(text: []const u8) bool {
        return std.mem.endsWith(u8, text, "Z");
    }

    pub fn countGhostSteps(self: *const Document, allocator: std.mem.Allocator) !u64 {
        var keyIter = self.map.keyIterator();

        var keys = std.ArrayList([]const u8).empty;
        defer keys.deinit(allocator);

        while (keyIter.next()) |k| {
            if (std.mem.endsWith(u8, k.*, "A")) {
                try keys.append(allocator, k.*);
            }
        }

        var total: u64 = 1;

        for (keys.items) |start| {
            total = lcm(total, self.countSteps(start, &endsWithZ));
        }

        return total;
    }
    pub fn deinit(self: *Document) void {
        self.map.deinit();
    }
};

fn parseDocument(input: []const u8, allocator: std.mem.Allocator) !Document {
    var it = std.mem.tokenizeAny(u8, input, "\r\n");
    var result = Document{
        .instructions = it.next().?,
        .map = std.StringHashMap(Node).init(allocator),
    };

    while (it.next()) |line| {
        var parts = std.mem.splitSequence(u8, line, " = ");
        const key = parts.first();
        var leftRight = std.mem.splitAny(u8, std.mem.trim(u8, parts.rest(), "()"), ",");
        try result.map.put(key, Node{
            .left = std.mem.trim(u8, leftRight.first(), " "),
            .right = std.mem.trim(u8, leftRight.rest(), " "),
        });
    }

    return result;
}

fn isZZZ(text: []const u8) bool {
    return std.mem.eql(u8, text, "ZZZ");
}

fn part1(input: []const u8, allocator: std.mem.Allocator) !u32 {
    var doc = try parseDocument(input, allocator);
    defer doc.deinit();

    return doc.countSteps("AAA", &isZZZ);
}

fn part2(input: []const u8, allocator: std.mem.Allocator) !u64 {
    var doc = try parseDocument(input, allocator);
    defer doc.deinit();
    return try doc.countGhostSteps(allocator);
}

test "part 1-a" {
    const exampleA =
        \\RL
        \\
        \\AAA = (BBB, CCC)
        \\BBB = (DDD, EEE)
        \\CCC = (ZZZ, GGG)
        \\DDD = (DDD, DDD)
        \\EEE = (EEE, EEE)
        \\GGG = (GGG, GGG)
        \\ZZZ = (ZZZ, ZZZ)
    ;
    try std.testing.expectEqual(@as(u32, 2), try part1(exampleA, std.testing.allocator));

    const exampleB =
        \\LLR
        \\
        \\AAA = (BBB, BBB)
        \\BBB = (AAA, ZZZ)
        \\ZZZ = (ZZZ, ZZZ)
    ;
    try std.testing.expectEqual(@as(u32, 6), try part1(exampleB, std.testing.allocator));
}

test "part 2" {
    const example =
        \\LR
        \\
        \\11A = (11B, XXX)
        \\11B = (XXX, 11Z)
        \\11Z = (11B, XXX)
        \\22A = (22B, XXX)
        \\22B = (22C, 22C)
        \\22C = (22Z, 22Z)
        \\22Z = (22B, 22B)
        \\XXX = (XXX, XXX)
    ;
    try std.testing.expectEqual(@as(u64, 6), try part2(example, std.testing.allocator));
}
