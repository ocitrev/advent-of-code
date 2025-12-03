const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/2025/day/3
    utils.printTitle(2025, 3, "Lobby");

    const m = utils.Monitor.init();
    defer m.deinit();
    const input = comptime utils.trimInput(@embedFile("input"));

    const p1 = part1(input);
    utils.printAnswer(1, p1);
    std.debug.assert(17155 == p1);

    const p2 = part2(input);
    utils.printAnswer(2, p2);
    std.debug.assert(169685670469164 == p2);
}

const Int = u64;

fn part1(input: []const u8) Int {
    var result: Int = 0;
    var lineIt = std.mem.tokenizeAny(u8, input, "\r\n");
    while (lineIt.next()) |line| {
        var jolts: Int = 0;
        for (0.., line) |i, _| {
            for (i + 1..line.len) |j| {
                const tens = std.fmt.charToDigit(line[i], 10) catch unreachable;
                const units = std.fmt.charToDigit(line[j], 10) catch unreachable;
                const num: Int = tens * 10 + units;

                if (num > jolts) {
                    jolts = num;
                }
            }
        }

        result += jolts;
    }

    return result;
}

fn part2(input: []const u8) Int {
    var result: Int = 0;
    var lineIt = std.mem.tokenizeAny(u8, input, "\r\n");

    while (lineIt.next()) |line| {
        var jolts: Int = 0;
        var buffer = [12]u8{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        var pos: usize = 0;
        var start: usize = 0;

        while (pos < 12) {
            var best: ?usize = null;
            const last = line.len - (11 - pos);

            if (start < last) {
                for (start.., line[start..last]) |i, c| {
                    if (c > buffer[pos]) {
                        buffer[pos] = c;
                        best = i;
                    }
                }
            }

            if (best) |n| {
                start = n + 1;
            } else {
                buffer[pos] = line[start];
                start += 1;
            }

            pos += 1;
        }

        jolts = std.fmt.parseInt(Int, &buffer, 10) catch unreachable;
        result += jolts;
    }

    return result;
}

test "parts 1,2" {
    const example =
        \\987654321111111
        \\811111111111119
        \\234234234234278
        \\818181911112111
    ;
    try std.testing.expectEqual(357, part1(example));
    try std.testing.expectEqual(3121910778619, part2(example));
}
