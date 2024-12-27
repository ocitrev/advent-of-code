const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/2024/day/25
    utils.printTitle(2024, 25, "Code Chronicle");

    const m = utils.Monitor.init();
    defer m.deinit();
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const ally = gpa.allocator();
    const input = comptime utils.trim_input(@embedFile("input"));

    const p1 = try part1(ally, input);
    utils.printAnswer(1, p1);
    std.debug.assert(3057 == p1);
}

const Int = i32;
const Type = enum { key, lock };
const Pins = @Vector(5, i8);

fn part1(ally: std.mem.Allocator, input: []const u8) !Int {
    var keys = std.ArrayList(Pins).init(ally);
    defer keys.deinit();
    var locks = std.ArrayList(Pins).init(ally);
    defer locks.deinit();

    var t: ?Type = null;
    var counter = Pins{ 0, 0, 0, 0, 0 };

    var lineIt = std.mem.splitAny(u8, input, "\r\n");
    while (lineIt.next()) |line| {
        if (line.len == 0) {
            switch (t.?) {
                .key => try keys.append(counter),
                .lock => try locks.append(counter),
            }

            t = null;
            continue;
        }

        if (t == null) {
            if (line[0] == '#') {
                t = .lock;
                counter = @splat(0);
            } else {
                t = .key;
                counter = @splat(-1);
            }
            continue;
        }

        for (0..5) |i| {
            if (line[i] == '#') {
                counter[i] += 1;
            }
        }
    }

    switch (t.?) {
        .key => try keys.append(counter),
        .lock => try locks.append(counter),
    }

    var valid: Int = 0;
    for (locks.items) |l| {
        for (keys.items) |k| {
            const clash = @as(Pins, @splat(5)) - k - l;

            if (@reduce(.Min, clash) >= 0) {
                valid += 1;
            }
        }
    }

    return valid;
}

test "part 1" {
    const ally = std.testing.allocator;
    const example =
        \\#####
        \\.####
        \\.####
        \\.####
        \\.#.#.
        \\.#...
        \\.....
        \\
        \\#####
        \\##.##
        \\.#.##
        \\...##
        \\...#.
        \\...#.
        \\.....
        \\
        \\.....
        \\#....
        \\#....
        \\#...#
        \\#.#.#
        \\#.###
        \\#####
        \\
        \\.....
        \\.....
        \\#.#..
        \\###..
        \\###.#
        \\###.#
        \\#####
        \\
        \\.....
        \\.....
        \\.....
        \\#....
        \\#.#..
        \\#.#.#
        \\#####
    ;
    try std.testing.expectEqual(3, try part1(ally, example));
}
