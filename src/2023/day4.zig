const std = @import("std");
const utils = @import("utils");
const Parser = utils.Parser;

pub fn main() !void {
    // https://adventofcode.com/2023/day/4
    utils.printTitle(2023, 4, "Scratchcards");

    const m = utils.Monitor.init();
    defer m.deinit();

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();
    const input = comptime utils.trimInput(@embedFile("input"));

    utils.printAnswer(1, try part1(input, allocator));
    utils.printAnswer(2, try part2(input, allocator));
}

const Card = struct {
    id: i32 = 0,
    instances: usize = 1,
    matchCount: usize = 0,

    pub fn parse(input: []const u8, allocator: anytype) !Card {
        var p = Parser.init(input);
        _ = p.nextScalar(' ');
        const id = p.nextScalar(':').?;
        const winner = p.nextScalar('|').?;
        const mine = p.rest();

        var winners = std.AutoHashMap(i32, void).init(allocator);
        defer winners.deinit();

        p = Parser.init(winner);
        while (p.nextScalar(' ')) |num| {
            try winners.put(try std.fmt.parseInt(i32, num, 10), {});
        }

        var matchCount: usize = 0;
        p = Parser.init(mine);
        while (p.nextScalar(' ')) |num| {
            if (winners.get(try std.fmt.parseInt(i32, num, 10))) |_| {
                matchCount += 1;
            }
        }

        return Card{ .id = try std.fmt.parseInt(u8, id, 10), .matchCount = matchCount };
    }

    pub fn getScore(self: *const @This()) !usize {
        if (self.matchCount == 0)
            return 0;
        return try std.math.powi(usize, 2, self.matchCount - 1);
    }

    pub fn duplicate(self: *@This(), count: usize) void {
        self.instances += count;
    }
};

test "Card.parse" {
    const c1 = try Card.parse("Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53", std.testing.allocator);
    try std.testing.expectEqual(@as(i32, 1), c1.id);
    const c2 = try Card.parse("Card 2: 13 32 20 16 61 | 61 30 68 82 17 32 24 19", std.testing.allocator);
    try std.testing.expectEqual(@as(i32, 2), c2.id);
}

fn part1(input: []const u8, allocator: std.mem.Allocator) !usize {
    var it = std.mem.tokenizeAny(u8, input, "\r\n");
    var total: usize = 0;
    while (it.next()) |line| {
        const c = try Card.parse(line, allocator);
        total += try c.getScore();
    }
    return total;
}

fn part2(input: []const u8, allocator: std.mem.Allocator) !usize {
    var cards = std.ArrayList(Card).init(allocator);
    var it = std.mem.tokenizeAny(u8, input, "\r\n");
    defer cards.deinit();
    while (it.next()) |line| {
        try cards.append(try Card.parse(line, allocator));
    }

    var i: usize = 0;
    while (i != cards.items.len) : (i += 1) {
        var next: usize = i + 1;
        const end = i + cards.items[i].matchCount + 1;
        while (next != end) : (next += 1) {
            cards.items[next].duplicate(cards.items[i].instances);
        }
    }

    var total: usize = 0;
    for (cards.items) |c| {
        total += c.instances;
    }

    return total;
}

test "part 1" {
    const example =
        \\Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53
        \\Card 2: 13 32 20 16 61 | 61 30 68 82 17 32 24 19
        \\Card 3:  1 21 53 59 44 | 69 82 63 72 16 21 14  1
        \\Card 4: 41 92 73 84 69 | 59 84 76 51 58  5 54 83
        \\Card 5: 87 83 26 28 32 | 88 30 70 12 93 22 82 36
        \\Card 6: 31 18 13 56 72 | 74 77 10 23 35 67 36 11
    ;
    try std.testing.expectEqual(@as(usize, 13), try part1(example, std.testing.allocator));
}

test "part 2" {
    const example =
        \\Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53
        \\Card 2: 13 32 20 16 61 | 61 30 68 82 17 32 24 19
        \\Card 3:  1 21 53 59 44 | 69 82 63 72 16 21 14  1
        \\Card 4: 41 92 73 84 69 | 59 84 76 51 58  5 54 83
        \\Card 5: 87 83 26 28 32 | 88 30 70 12 93 22 82 36
        \\Card 6: 31 18 13 56 72 | 74 77 10 23 35 67 36 11
    ;
    try std.testing.expectEqual(@as(usize, 30), try part2(example, std.testing.allocator));
}

test "parse test" {
    const input = "Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53";
    _ = input;
}
