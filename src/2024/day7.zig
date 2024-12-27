const std = @import("std");
const utils = @import("utils");
const int = u64;
const kMaxNumbers: usize = 12;

pub fn main() !void {
    // https://adventofcode.com/2024/day/7
    utils.printTitle(2024, 7, "Bridge Repair");

    const m = utils.Monitor.init();
    defer m.deinit();
    const input = comptime utils.trim_input(@embedFile("input"));

    const p1 = try part1(input);
    utils.printAnswer(1, p1);
    std.debug.assert(3312271365652 == p1);

    const p2 = try part2(input);
    utils.printAnswer(2, p2);
    std.debug.assert(509463489296712 == p2);
}

const ValidateFn = fn (expected: int, numbers: []int, comptime concat: bool) bool;

fn validate(input: []const u8, isValidFn: ValidateFn, comptime concat: bool) !int {
    var sum: int = 0;
    var numbersBuffer: [kMaxNumbers]int = undefined;
    var lineIt = std.mem.tokenizeAny(u8, input, "\r\n");
    while (lineIt.next()) |line| {
        var numbersIt = std.mem.tokenizeScalar(u8, line, ':');
        const expectedText = numbersIt.next().?;
        const expected = std.fmt.parseInt(int, expectedText, 10) catch unreachable;

        var numbers = std.ArrayListUnmanaged(int).initBuffer(&numbersBuffer);
        numbersIt = std.mem.tokenizeScalar(u8, numbersIt.rest(), ' ');
        while (numbersIt.next()) |num| {
            const n = std.fmt.parseInt(int, num, 10) catch unreachable;
            numbers.appendAssumeCapacity(n);
        }

        if (isValidFn(expected, numbers.items, concat)) {
            sum += expected;
        }
    }

    return sum;
}

fn isValid(expected: int, numbers: []int, comptime concat: bool) bool {
    if (numbers.len == 1)
        return expected == numbers[0];

    if (numbers[0] > expected) {
        return false;
    }

    var reducedBuffer: [kMaxNumbers]int = undefined;
    var reduced = std.ArrayListUnmanaged(int).initBuffer(&reducedBuffer);
    reduced.appendAssumeCapacity(numbers[0] + numbers[1]);

    for (numbers[2..]) |n| {
        reduced.appendAssumeCapacity(n);
    }

    if (isValid(expected, reduced.items, concat)) {
        return true;
    }

    reduced.clearRetainingCapacity();
    reduced.appendAssumeCapacity(numbers[0] * numbers[1]);

    for (numbers[2..]) |n| {
        reduced.appendAssumeCapacity(n);
    }

    if (isValid(expected, reduced.items, concat)) {
        return true;
    }

    if (concat) {
        const pow = std.math.powi(int, 10, std.math.log10_int(numbers[1]) + 1) catch unreachable;
        const newNumber = numbers[0] * pow + numbers[1];
        reduced.clearRetainingCapacity();
        reduced.appendAssumeCapacity(newNumber);

        for (numbers[2..]) |n| {
            reduced.appendAssumeCapacity(n);
        }

        if (isValid(expected, reduced.items, concat)) {
            return true;
        }
    }

    return false;
}

fn part1(input: []const u8) !int {
    return try validate(input, isValid, false);
}

test "part 1" {
    const example =
        \\190: 10 19
        \\3267: 81 40 27
        \\83: 17 5
        \\156: 15 6
        \\7290: 6 8 6 15
        \\161011: 16 10 13
        \\192: 17 8 14
        \\21037: 9 7 18 13
        \\292: 11 6 16 20
    ;
    try std.testing.expectEqual(3749, try part1(example));
}

fn part2(input: []const u8) !int {
    return validate(input, isValid, true);
}

test "part 2" {
    const example =
        \\190: 10 19
        \\3267: 81 40 27
        \\83: 17 5
        \\156: 15 6
        \\7290: 6 8 6 15
        \\161011: 16 10 13
        \\192: 17 8 14
        \\21037: 9 7 18 13
        \\292: 11 6 16 20
    ;
    try std.testing.expectEqual(11387, try part2(example));
}
