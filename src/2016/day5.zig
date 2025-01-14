const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/2016/day/5
    utils.printTitle(2016, 5, "How About a Nice Game of Chess?");

    const m = utils.Monitor.init();
    defer m.deinit();

    const input = comptime utils.trimInput(@embedFile("input"));
    const result = try generate(input);
    utils.printAnswer(1, &result.part1);
    utils.printAnswer(2, &result.part2);
}

const Result = struct {
    part1: [8]u8 = undefined,
    part2: [8]u8 = .{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
};

fn digitToChar(value: u8) u8 {
    return std.fmt.digitToChar(value, std.fmt.Case.lower);
}

fn startsWith00000(digest: []const u8) bool {
    return digest[0] == 0 and digest[1] == 0 and digest[2] & 0xf0 == 0;
}

fn generate(input: []const u8) !Result {
    var digest: [16]u8 = undefined;
    var result = Result{};
    var count1: u8 = 0;
    var count2: u8 = 0;
    var num: u32 = 1;
    var buffer: [100]u8 = undefined;

    while (count1 < 8 or count2 < 8) : (num += 1) {
        const door_id = try std.fmt.bufPrint(&buffer, "{s}{}", .{ input, num });
        std.crypto.hash.Md5.hash(door_id, digest[0..], .{});

        if (startsWith00000(&digest)) {
            if (count1 < 8) {
                result.part1[count1] = digitToChar(digest[2] & 0x0f);
                count1 += 1;
            }

            if (count2 < 8) {
                const pos = digest[2] & 0x0f;
                if (pos < 8 and result.part2[pos] == ' ') {
                    result.part2[pos] = digitToChar((digest[3] & 0xf0) >> 4);
                    count2 += 1;
                }
            }
        }
    }

    return result;
}

test "part 1,2" {
    const example =
        \\abc
    ;
    const result = try generate(example);
    try std.testing.expectEqualStrings("18f47a30", &result.part1);
    try std.testing.expectEqualStrings("05ace8e3", &result.part2);
}
