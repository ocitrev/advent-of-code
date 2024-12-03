const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    const input = comptime utils.trim_input(@embedFile("input"));
    // https://adventofcode.com/2024/day/3
    std.debug.print("Day 3, 2024: Mull It Over\n", .{});

    const p1 = try part1(input);
    std.debug.print("  Part 1: {}\n", .{p1});
    std.debug.assert(167090022 == p1);

    const p2 = try part2(input);
    std.debug.print("  Part 2: {}\n", .{p2});
    std.debug.assert(89823704 == p2);
}

const State = enum {
    num1,
    num2,
    end,
};

const Result = struct {
    len: usize,
    product: i64,
};

fn is_valid(input: []const u8) !?Result {
    var num1: [3]u8 = undefined;
    var num1_len: usize = 0;

    var num2: [3]u8 = undefined;
    var num2_len: usize = 0;

    if (!std.mem.eql(u8, input[0..4], "mul(")) {
        return null;
    }

    var state = State.num1;
    var len: usize = 0;

    outer: for (0.., input[4..]) |i, c| {
        switch (state) {
            .num1 => {
                switch (c) {
                    ',' => state = .num2,
                    '0'...'9' => {
                        if (num1_len == 3) return null;
                        num1[num1_len] = c;
                        num1_len += 1;
                    },
                    else => return null,
                }
            },
            .num2 => {
                switch (c) {
                    ')' => {
                        state = .end;
                        len = i;
                        break :outer;
                    },
                    '0'...'9' => {
                        if (num2_len == 3) return null;
                        num2[num2_len] = c;
                        num2_len += 1;
                    },
                    else => return null,
                }
            },
            else => {},
        }
    }

    if (state != .end) {
        return null;
    }

    const a = try std.fmt.parseInt(i64, num1[0..num1_len], 10);
    const b = try std.fmt.parseInt(i64, num2[0..num2_len], 10);
    return Result{ .len = len + 4, .product = a * b };
}

fn part1(input: []const u8) !i64 {
    var skip: usize = 0;
    var sum: i64 = 0;

    for (0.., input) |i, c| {
        if (skip != 0) {
            skip -= 1;
            continue;
        }

        if (c == 'm') {
            const chunk = input[i..@min(input.len, i + 13)];
            if (try is_valid(chunk)) |result| {
                skip = result.len - 1;
                sum += result.product;
            }
        }
    }

    return sum;
}

test "part 1" {
    const example =
        \\xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))
    ;
    comptime try std.testing.expectEqual(@as(i32, 161), try part1(example));
}

fn part2(input: []const u8) !i64 {
    var skip: usize = 0;
    var sum: i64 = 0;
    var enabled = true;

    for (0.., input) |i, c| {
        if (skip != 0) {
            skip -= 1;
            continue;
        }

        if (c == 'd') {
            if (std.mem.eql(u8, "do()", input[i .. i + 4])) {
                enabled = true;
                skip = 3;
            } else if (std.mem.eql(u8, "don't()", input[i .. i + 7])) {
                enabled = false;
                skip = 6;
            }

            continue;
        }

        if (!enabled) {
            continue;
        }

        if (c == 'm') {
            const chunk = input[i..@min(input.len, i + 13)];
            if (try is_valid(chunk)) |result| {
                skip = result.len - 1;
                sum += result.product;
            }
        }
    }

    return sum;
}

test "part 2" {
    const example =
        \\xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))
    ;
    comptime try std.testing.expectEqual(48, try part2(example));
}
