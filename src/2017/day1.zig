const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/2017/day/1
    utils.printTitle(2017, 1, "Inverse Captcha");

    const m = utils.Monitor.init();
    defer m.deinit();
    const input = comptime utils.trimInput(@embedFile("input"));

    @setEvalBranchQuota(5_000);
    const p1 = comptime part1(input);
    utils.printAnswer(1, p1);
    std.debug.assert(1150 == p1);

    const p2 = comptime part2(input);
    utils.printAnswer(2, p2);
    std.debug.assert(1064 == p2);
}

fn part1(input: []const u8) i32 {
    var sum: i32 = 0;
    for (0.., input) |i, c| {
        const next = input[@mod(i + 1, input.len)];
        if (c == next) {
            sum += std.fmt.charToDigit(c, 10) catch unreachable;
        }
    }
    return sum;
}

test part1 {
    try comptime std.testing.expectEqual(3, part1("1122"));
    try comptime std.testing.expectEqual(4, part1("1111"));
    try comptime std.testing.expectEqual(0, part1("1234"));
    try comptime std.testing.expectEqual(9, part1("91212129"));
}

fn part2(input: []const u8) i32 {
    var sum: i32 = 0;
    const half = input.len / 2;
    for (input, 0..) |c, i| {
        const next = input[@mod(i + half, input.len)];
        if (c == next) {
            sum += std.fmt.charToDigit(c, 10) catch unreachable;
        }
    }
    return sum;
}

test part2 {
    try comptime std.testing.expectEqual(6, part2("1212"));
    try comptime std.testing.expectEqual(0, part2("1221"));
    try comptime std.testing.expectEqual(4, part2("123425"));
    try comptime std.testing.expectEqual(12, part2("123123"));
    try comptime std.testing.expectEqual(4, part2("12131415"));
}
