const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/2023/day/1
    utils.printTitle(2023, 1, "Trebuchet?!");

    const m = utils.Monitor.init();
    defer m.deinit();

    @setEvalBranchQuota(1500000);

    const input = comptime utils.trimInput(@embedFile("input"));
    utils.printAnswer(1, comptime part1(input));
    utils.printAnswer(2, comptime part2(input));
}

fn getDigit(text: []const u8) ?i32 {
    if (text[0] >= '0' and text[0] <= '9') {
        return text[0] - '0';
    } else if (std.mem.startsWith(u8, text, "one")) {
        return 1;
    } else if (std.mem.startsWith(u8, text, "two")) {
        return 2;
    } else if (std.mem.startsWith(u8, text, "three")) {
        return 3;
    } else if (std.mem.startsWith(u8, text, "four")) {
        return 4;
    } else if (std.mem.startsWith(u8, text, "five")) {
        return 5;
    } else if (std.mem.startsWith(u8, text, "six")) {
        return 6;
    } else if (std.mem.startsWith(u8, text, "seven")) {
        return 7;
    } else if (std.mem.startsWith(u8, text, "eight")) {
        return 8;
    } else if (std.mem.startsWith(u8, text, "nine")) {
        return 9;
    }

    return null;
}

fn findFirstDigit(text: []const u8) i32 {
    for (text, 0..) |_, i| {
        if (getDigit(text[i..])) |digit| {
            return digit;
        }
    }

    return 0;
}

fn findLastDigit(text: []const u8) i32 {
    var i: usize = text.len;
    while (i > 0) {
        i -= 1;
        if (getDigit(text[i..])) |digit| {
            return digit;
        }
    }

    return 0;
}

fn part1(text: []const u8) i32 {
    const textTrimmed = std.mem.trim(u8, text, "\r\n");
    var it = std.mem.tokenizeAny(u8, textTrimmed, "\r\n");

    var sum: i32 = 0;
    while (it.next()) |line| {
        const first = std.mem.indexOfAny(u8, line, "123456789").?;
        const last = std.mem.lastIndexOfAny(u8, line, "123456789").?;
        const num = (line[first] - '0') * 10 + (line[last] - '0');
        sum += num;
    }

    return sum;
}

fn part2(text: []const u8) i32 {
    const textTrimmed = std.mem.trim(u8, text, "\r\n");
    var it = std.mem.tokenizeAny(u8, textTrimmed, "\r\n");
    var sum: i32 = 0;
    while (it.next()) |line| {
        const first = findFirstDigit(line);
        const last = findLastDigit(line);
        sum += first * 10 + last;
    }

    return sum;
}

test "part 1" {
    const example =
        \\1abc2
        \\pqr3stu8vwx
        \\a1b2c3d4e5f
        \\treb7uchet
    ;
    try std.testing.expectEqual(142, comptime part1(example));
}

test "part 2" {
    const example =
        \\two1nine
        \\eightwothree
        \\abcone2threexyz
        \\xtwone3four
        \\4nineeightseven2
        \\zoneight234
        \\7pqrstsixteen
    ;
    @setEvalBranchQuota(2000);
    try std.testing.expectEqual(281, comptime part2(example));
}
