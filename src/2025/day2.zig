const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/2025/day/2
    utils.printTitle(2025, 2, "Gift Shop");

    const m = utils.Monitor.init();
    defer m.deinit();
    const input = comptime utils.trimInput(@embedFile("input"));

    const p1 = part1(input);
    utils.printAnswer(1, p1);
    std.debug.assert(8576933996 == p1);

    const p2 = part2(input);
    utils.printAnswer(2, p2);
    std.debug.assert(25663320831 == p2);
}

const Int = u64;

fn part1(input: []const u8) Int {
    var result: Int = 0;
    var lineIt = std.mem.tokenizeAny(u8, input, ",\r\n");
    while (lineIt.next()) |line| {
        var it = std.mem.splitScalar(u8, line, '-');
        const low = std.fmt.parseInt(usize, it.next().?, 10) catch unreachable;
        const high = std.fmt.parseInt(usize, it.next().?, 10) catch unreachable;

        for (low..(high + 1)) |v| {
            const nb = utils.nbDigits(usize, v);
            if (nb & 1 == 0) {
                const p = std.math.pow(usize, 10, nb / 2);
                const left = @divTrunc(v, p);
                const right = @rem(v, p);

                if (left == right) {
                    result += v;
                }
            }
        }
    }

    return result;
}

fn part2(input: []const u8) Int {
    var buffer: [100]u8 = undefined;
    var result: Int = 0;
    var lineIt = std.mem.tokenizeAny(u8, input, ",\r\n");
    while (lineIt.next()) |line| {
        var it = std.mem.splitScalar(u8, line, '-');
        const low = std.fmt.parseInt(usize, it.next().?, 10) catch unreachable;
        const high = std.fmt.parseInt(usize, it.next().?, 10) catch unreachable;

        for (low..(high + 1)) |v| {
            const text = std.fmt.bufPrint(&buffer, "{}", .{v}) catch unreachable;

            repeatCheck: for (1..((text.len / 2) + 1)) |len| {
                if (text.len % len != 0) continue;
                const needle = text[0..len];
                var itWindow = std.mem.window(u8, text[len..], len, len);
                while (itWindow.next()) |chunk| {
                    if (!std.mem.eql(u8, needle, chunk)) {
                        continue :repeatCheck;
                    }
                }

                result += v;
                break;
            }
        }
    }

    return result;
}

test "parts 1,2" {
    const example =
        \\11-22,95-115,998-1012,1188511880-1188511890,222220-222224,
        \\1698522-1698528,446443-446449,38593856-38593862,565653-565659,
        \\824824821-824824827,2121212118-2121212124
    ;
    try std.testing.expectEqual(1227775554, part1(example));
    try std.testing.expectEqual(4174379265, part2(example));
}
