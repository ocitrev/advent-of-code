const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/2025/day/1
    utils.printTitle(2025, 1, "Secret Entrance");

    const m = utils.Monitor.init();
    defer m.deinit();
    const input = comptime utils.trimInput(@embedFile("input"));

    const result = run(input);
    utils.printAnswer(1, result[0]);
    std.debug.assert(992 == result[0]);

    utils.printAnswer(2, result[1]);
    std.debug.assert(6133 == result[1]);
}

fn run(input: []const u8) struct { u32, u32 } {
    var result1: u32 = 0;
    var result2: u32 = 0;
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
                result2 += 1;
            }
        }

        if (start == 0) {
            result1 += 1;
        }
    }

    return .{ result1, result2 };
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
    const result = comptime run(example);
    try std.testing.expectEqual(3, result[0]);
    try std.testing.expectEqual(6, result[1]);
}
