const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/2025/day/1
    utils.printTitle(2025, 1, "Secret Entrance");

    const m = utils.Monitor.init();
    defer m.deinit();
    const input = comptime utils.trimInput(@embedFile("input"));

    const p1 = part1(input);
    utils.printAnswer(1, p1);
    std.debug.assert(992 == p1);

    const p2 = part2(input);
    utils.printAnswer(2, p2);
    std.debug.assert(6133 == p2);
}

fn part1(input: []const u8) u32 {
    var result: u32 = 0;
    var start: i32 = 50;

    var lineIt = std.mem.tokenizeAny(u8, input, "\r\n");
    while (lineIt.next()) |line| {
        const left = line[0] == 'L';
        const count = std.fmt.parseInt(i32, line[1..], 10) catch unreachable;
        const value = if (left) -count else count;
        start = @mod(start + value, 100);

        if (start == 0) {
            result += 1;
        }
    }

    return result;
}

fn part2(input: []const u8) u32 {
    var result: u32 = 0;
    var start: i32 = 50;

    var lineIt = std.mem.tokenizeAny(u8, input, "\r\n");
    while (lineIt.next()) |line| {
        const count = std.fmt.parseInt(usize, line[1..], 10) catch unreachable;
        const inc: i32 = if (line[0] == 'L') -1 else 1;

        for (0..count) |_| {
            start += inc;
            if (start == -1) {
                start = 99;
            } else if (start == 100) {
                start = 0;
            }

            if (start == 0) {
                result += 1;
            }
        }
    }

    return result;
}

test "parts 1,2" {
    const example =
        \\L68
        \\L30
        \\R48
        \\L5
        \\R60
        \\L55
        \\L1
        \\L99
        \\R14
        \\L82
    ;
    @setEvalBranchQuota(2000);
    try std.testing.expectEqual(3, comptime part1(example));
    try std.testing.expectEqual(6, comptime part2(example));
}
