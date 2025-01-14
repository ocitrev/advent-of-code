const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/2023/day/2
    utils.printTitle(2023, 2, "Cube Conundrum");

    const m = utils.Monitor.init();
    defer m.deinit();

    const input = comptime utils.trimInput(@embedFile("input"));

    utils.printAnswer(1, try part1(input));
    utils.printAnswer(2, try part2(input));
}

const Cubes = struct {
    red: i32 = 0,
    green: i32 = 0,
    blue: i32 = 0,

    pub fn parse(text: []const u8) !Cubes {
        var result = Cubes{};
        var it = std.mem.tokenizeScalar(u8, text, ',');

        while (it.next()) |cube| {
            const trimmedCube = std.mem.trim(u8, cube, " \t\r\n");
            const pos = std.mem.indexOfScalar(u8, trimmedCube, ' ').?;
            const name = trimmedCube[pos + 1 ..];
            const value = try std.fmt.parseInt(i32, trimmedCube[0..pos], 10);

            if (std.mem.eql(u8, name, "red")) {
                result.red += value;
            } else if (std.mem.eql(u8, name, "green")) {
                result.green += value;
            } else if (std.mem.eql(u8, name, "blue")) {
                result.blue += value;
            }
        }

        return result;
    }

    pub fn getPower(self: *const Cubes) i32 {
        return self.red * self.green * self.blue;
    }
};

test "Cubes.create" {
    const c = comptime try Cubes.parse("1 red, 2 green, 3 blue");
    try std.testing.expectEqual(1, c.red);
    try std.testing.expectEqual(2, c.green);
    try std.testing.expectEqual(3, c.blue);
}

fn isGamePossible(game: []const u8, bag: Cubes) !bool {
    var drawIt = std.mem.tokenizeScalar(u8, game, ';');
    while (drawIt.next()) |draw| {
        const cubes = try Cubes.parse(draw);
        if (cubes.red > bag.red or cubes.green > bag.green or cubes.blue > bag.blue) {
            return false;
        }
    }

    return true;
}

fn part1(input: []const u8) !i32 {
    var it = std.mem.tokenizeAny(u8, input, "\r\n");
    var total: i32 = 0;
    const bag = Cubes{ .red = 12, .green = 13, .blue = 14 };

    while (it.next()) |line| {
        const pos = std.mem.indexOfScalar(u8, line, ':').?;
        if (try isGamePossible(line[pos + 1 ..], bag)) {
            const gameId = line[0..pos];
            const spacePos = std.mem.indexOfScalar(u8, gameId, ' ').?;
            total += try std.fmt.parseInt(i32, gameId[spacePos + 1 ..], 10);
        }
    }
    return total;
}

fn part2(input: []const u8) !i32 {
    var it = std.mem.tokenizeAny(u8, input, "\r\n");
    var total: i32 = 0;

    while (it.next()) |line| {
        const pos = std.mem.indexOfScalar(u8, line, ':').?;
        var drawIt = std.mem.tokenizeScalar(u8, line[pos + 1 ..], ';');
        var max = Cubes{};
        while (drawIt.next()) |draw| {
            const cubes = try Cubes.parse(draw);
            max.red = @max(max.red, cubes.red);
            max.green = @max(max.green, cubes.green);
            max.blue = @max(max.blue, cubes.blue);
        }

        total += max.getPower();
    }

    return total;
}

test "part 1" {
    @setEvalBranchQuota(10000);
    const example =
        \\Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green
        \\Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue
        \\Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red
        \\Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red
        \\Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green
    ;

    try std.testing.expectEqual(8, comptime try part1(example));
}

test "part 2" {
    @setEvalBranchQuota(10000);
    const example =
        \\Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green
        \\Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue
        \\Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red
        \\Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red
        \\Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green
    ;

    try std.testing.expectEqual(2286, comptime try part2(example));
}
