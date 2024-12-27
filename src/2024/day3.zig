const std = @import("std");
const utils = @import("utils");

pub fn main() void {
    // https://adventofcode.com/2024/day/3
    utils.printTitle(2024, 3, "Mull It Over");

    const m = utils.Monitor.init();
    defer m.deinit();
    const input = comptime utils.trim_input(@embedFile("input"));

    @setEvalBranchQuota(80_000);
    const result = comptime run(input);
    utils.printAnswer(1, result.part1);
    comptime std.debug.assert(167090022 == result.part1);

    utils.printAnswer(2, result.part2);
    comptime std.debug.assert(89823704 == result.part2);
}

const State = enum {
    num1,
    num2,
    end,
};

const MulResult = struct {
    len: usize,
    product: i64,
};

fn is_valid(input: []const u8) ?MulResult {
    var num1: [3]u8 = undefined;
    var num1_len: usize = 0;

    var num2: [3]u8 = undefined;
    var num2_len: usize = 0;

    if (!std.mem.eql(u8, input[0..4], "mul(")) {
        return null;
    }

    var state = State.num1;
    var len: usize = 0;

    outer: for (4.., input[4..]) |i, c| {
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

    const a = std.fmt.parseInt(i64, num1[0..num1_len], 10) catch unreachable;
    const b = std.fmt.parseInt(i64, num2[0..num2_len], 10) catch unreachable;
    return .{ .len = len, .product = a * b };
}

fn run(input: []const u8) utils.Result(i64) {
    var skip: usize = 0;
    var result = utils.Result(i64).init();
    var enabled = true;

    for (0.., input) |i, c| {
        if (skip != 0) {
            skip -= 1;
            continue;
        }

        switch (c) {
            'd' => {
                const do = "do()";
                const dont = "don't()";
                if (std.mem.startsWith(u8, input[i..], do)) {
                    enabled = true;
                    skip = do.len - 1;
                } else if (std.mem.startsWith(u8, input[i..], dont)) {
                    enabled = false;
                    skip = dont.len - 1;
                }
            },
            'm' => {
                const mul_max_len = "mul(XXX,XXX)".len;
                const chunk = input[i..@min(input.len, i + mul_max_len)];
                if (is_valid(chunk)) |r| {
                    skip = r.len - 1;
                    result.part1 += r.product;

                    if (enabled) {
                        result.part2 += r.product;
                    }
                }
            },
            else => {},
        }
    }

    return result;
}

test "part 1" {
    const example =
        \\xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))
    ;
    comptime try std.testing.expectEqual(@as(i32, 161), run(example).part1);
}

test "part 2" {
    const example =
        \\xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))
    ;
    comptime try std.testing.expectEqual(48, run(example).part2);
}
