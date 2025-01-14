const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/2024/day/13
    utils.printTitle(2024, 13, "Claw Contraption");

    const m = utils.Monitor.init();
    defer m.deinit();
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const ally = gpa.allocator();
    const input = comptime utils.trimInput(@embedFile("input"));

    const p1 = try part1(input, ally);
    utils.printAnswer(1, p1);
    std.debug.assert(38839 == p1);

    const p2 = try part2(input, ally);
    utils.printAnswer(2, p2);
    std.debug.assert(75200131617108 == p2);
}

const Int = i64;
const Point2d = utils.Point2d(Int);
const Game = struct {
    a: Point2d,
    b: Point2d,
    prize: Point2d,

    fn resolve(self: *const @This(), offset: Int) Int {
        // Button A: X+94, Y+34
        // Button B: X+22, Y+67
        // Prize: X=8400, Y=5400
        // 94A + 22B = 8400
        // 34A + 67B = 5400
        // A = (8400 * 67 - 34 * 5400) / (94 * 67 - 22 * 34)
        // B = (94 * 5400 - 8400 * 22) / (94 * 67 - 22 * 34)

        const p = self.prize.add(offset, offset);
        const numeratorA = (p.x * self.b.y) - (self.b.x * p.y);
        const numeratorB = (self.a.x * p.y) - (p.x * self.a.y);
        const denumerator = (self.a.x * self.b.y) - (self.a.y * self.b.x);
        const a: Int = @divTrunc(numeratorA, denumerator);
        const b: Int = @divTrunc(numeratorB, denumerator);

        // p == game.a * a + game.b * b
        if (p.eql(self.a.mul(a).addp(self.b.mul(b)))) {
            return a * 3 + b;
        }

        return 0;
    }
};

fn parseGames(input: []const u8, ally: std.mem.Allocator) ![]Game {
    var games = std.ArrayList(Game).init(ally);
    defer games.deinit();

    var lineIt = std.mem.tokenizeAny(u8, input, "\r\n");
    var a: ?Point2d = null;
    var b: ?Point2d = null;
    var prize: ?Point2d = null;

    while (lineIt.next()) |line| {
        const Button = "Button ";
        const ButtonAny = Button ++ "_:";
        const Prize = "Prize: ";

        if (std.mem.startsWith(u8, line, Button)) {
            var it = std.mem.splitScalar(u8, line[ButtonAny.len..], ',');
            const p = Point2d{
                .x = try std.fmt.parseInt(Int, std.mem.trim(u8, it.next().?, " ")[1..], 10),
                .y = try std.fmt.parseInt(Int, std.mem.trim(u8, it.next().?, " ")[1..], 10),
            };

            switch (line[Button.len]) {
                'A' => a = p,
                'B' => b = p,
                else => unreachable,
            }
        } else if (std.mem.startsWith(u8, line, Prize)) {
            var it = std.mem.splitScalar(u8, line[Prize.len..], ',');
            prize = Point2d{
                .x = try std.fmt.parseInt(Int, std.mem.trim(u8, it.next().?, " ")[2..], 10),
                .y = try std.fmt.parseInt(Int, std.mem.trim(u8, it.next().?, " ")[2..], 10),
            };
        }

        if (a != null and b != null and prize != null) {
            try games.append(Game{ .a = a.?, .b = b.?, .prize = prize.? });
            a = null;
            b = null;
            prize = null;
        }
    }

    return games.toOwnedSlice();
}

fn part1(input: []const u8, ally: std.mem.Allocator) !Int {
    const games = try parseGames(input, ally);
    defer ally.free(games);

    var sum: Int = 0;
    for (games) |g| {
        sum += g.resolve(0);
    }

    return sum;
}

fn part2(input: []const u8, ally: std.mem.Allocator) !Int {
    const games = try parseGames(input, ally);
    defer ally.free(games);

    var sum: Int = 0;
    const offset: Int = 10_000_000_000_000;

    for (games) |g| {
        sum += g.resolve(offset);
    }

    return sum;
}

test "parts" {
    const example =
        \\Button A: X+94, Y+34
        \\Button B: X+22, Y+67
        \\Prize: X=8400, Y=5400
        \\
        \\Button A: X+26, Y+66
        \\Button B: X+67, Y+21
        \\Prize: X=12748, Y=12176
        \\
        \\Button A: X+17, Y+86
        \\Button B: X+84, Y+37
        \\Prize: X=7870, Y=6450
        \\
        \\Button A: X+69, Y+23
        \\Button B: X+27, Y+71
        \\Prize: X=18641, Y=10279
    ;
    try std.testing.expectEqual(480, try part1(example, std.testing.allocator));
    try std.testing.expectEqual(875318608908, try part2(example, std.testing.allocator));
}
