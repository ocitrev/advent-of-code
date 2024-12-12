const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/2019/day/1
    std.debug.print("Day 1, 2019: The Tyranny of the Rocket Equation\n", .{});

    const m = utils.Monitor.init();
    defer m.deinit();
    const input = comptime utils.trim_input(@embedFile("input"));

    const p1 = try part1(input);
    std.debug.print("  Part 1: {}\n", .{p1});
    std.debug.assert(3538016 == p1);

    const p2 = try part2(input);
    std.debug.print("  Part 2: {}\n", .{p2});
    std.debug.assert(5304147 == p2);
}

fn getFuel(mass: i32) i32 {
    return @divFloor(mass, 3) - 2;
}

fn part1(input: []const u8) !i32 {
    var sum: i32 = 0;
    var it = std.mem.tokenizeAny(u8, input, "\r\n");
    while (it.next()) |line| {
        sum += getFuel(try std.fmt.parseInt(i32, line, 10));
    }

    return sum;
}

test "part 1" {
    comptime try std.testing.expectEqual(2, getFuel(12));
    comptime try std.testing.expectEqual(2, getFuel(14));
    comptime try std.testing.expectEqual(654, getFuel(1969));
    comptime try std.testing.expectEqual(33583, getFuel(100756));
}

fn getRealFuel(mass: i32) i32 {
    var fuel = getFuel(mass);

    if (fuel > 0) {
        return fuel + getRealFuel(fuel);
    }

    if (fuel < 0) {
        fuel = 0;
    }

    return fuel;
}

fn part2(input: []const u8) !i32 {
    var sum: i32 = 0;
    var it = std.mem.tokenizeAny(u8, input, "\r\n");
    while (it.next()) |line| {
        sum += getRealFuel(try std.fmt.parseInt(i32, line, 10));
    }

    return sum;
}

test "part 2" {
    comptime try std.testing.expectEqual(2, getRealFuel(12));
    comptime try std.testing.expectEqual(2, getRealFuel(14));
    comptime try std.testing.expectEqual(966, getRealFuel(1969));
    comptime try std.testing.expectEqual(50346, getRealFuel(100756));
}
