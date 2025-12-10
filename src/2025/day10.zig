const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/2025/day/10
    utils.printTitle(2025, 10, "Factory");

    const m = utils.Monitor.init();
    defer m.deinit();
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const ally = gpa.allocator();
    const input = comptime utils.trimInput(@embedFile("input"));

    const p1 = try part1(ally, input);
    utils.printAnswer(1, p1);
    std.debug.assert(434 == p1);

    const p2 = try part2(ally, input);
    utils.printAnswer(2, p2);
    // std.debug.assert(0 == p2);
}

const Int = u64;

const Machine = struct {
    lights: u16,
    buttons: []u16,
    joltages: []u16,
    ally: std.mem.Allocator,

    fn parse(ally: std.mem.Allocator, line: []const u8) !Machine {
        var lights: u16 = 0;
        var buttonList = std.ArrayList(u16).empty;
        var joltages = std.ArrayList(u16).empty;
        var it = std.mem.splitScalar(u8, line, ' ');
        var lightText = it.next().?;

        for (0.., lightText[1 .. lightText.len - 1]) |i, c| {
            if (c == '#') {
                lights |= @as(u16, 1) << @intCast(i);
            }
        }

        while (it.next()) |value| {
            if (value[0] == '(') {
                var buttonIt = std.mem.splitScalar(u8, value[1 .. value.len - 1], ',');
                var buttons: u16 = 0;

                while (buttonIt.next()) |btnStr| {
                    const btn = std.fmt.parseInt(u16, btnStr, 10) catch @panic("Invalid input");
                    buttons |= @as(u16, 1) << @intCast(btn);
                }
                try buttonList.append(ally, buttons);
            } else if (value[0] == '{') {
                var joltageIt = std.mem.splitScalar(u8, value[1 .. value.len - 1], ',');
                while (joltageIt.next()) |j| {
                    try joltages.append(ally, std.fmt.parseInt(u16, j, 10) catch @panic("Invalid input"));
                }
            }
        }

        return Machine{
            .lights = lights,
            .buttons = try buttonList.toOwnedSlice(ally),
            .joltages = try joltages.toOwnedSlice(ally),
            .ally = ally,
        };
    }

    fn deinit(self: *@This()) void {
        self.ally.free(self.buttons);
        self.ally.free(self.joltages);
    }

    fn countMinimumButtonPresses(self: *const @This()) Int {
        var count: Int = 0;

        for (0..@as(u16, 1) << @intCast(self.buttons.len)) |i| {
            var value: u16 = 0;
            var presses: Int = 0;

            for (0.., self.buttons) |j, b| {
                if ((i & (@as(u16, 1) << @intCast(j))) != 0) {
                    value ^= b;
                    presses += 1;
                }
            }

            if (self.lights == value) {
                if (count == 0 or presses < count) {
                    count = presses;
                }
            }
        }

        return count;
    }
};

const Machines = struct {
    list: []Machine,
    ally: std.mem.Allocator,

    fn parse(ally: std.mem.Allocator, input: []const u8) !Machines {
        var machines = std.ArrayList(Machine).empty;
        var lineIt = std.mem.tokenizeAny(u8, input, "\r\n");

        while (lineIt.next()) |line| {
            try machines.append(ally, try Machine.parse(ally, line));
        }

        return .{
            .list = try machines.toOwnedSlice(ally),
            .ally = ally,
        };
    }

    fn deinit(self: *@This()) void {
        for (self.list) |*machine| {
            machine.deinit();
        }
        self.ally.free(self.list);
    }

    fn part1(self: *const @This()) Int {
        var total: Int = 0;
        for (self.list) |machine| {
            total += machine.countMinimumButtonPresses();
        }
        return total;
    }

    fn part2(self: *const @This()) Int {
        _ = self;
        return 0;
    }
};

fn part1(ally: std.mem.Allocator, input: []const u8) !Int {
    var machines = try Machines.parse(ally, input);
    defer machines.deinit();
    return machines.part1();
}

fn part2(ally: std.mem.Allocator, input: []const u8) !Int {
    var machines = try Machines.parse(ally, input);
    defer machines.deinit();
    return machines.part2();
}

test "parts 1,2" {
    const ally = std.testing.allocator;
    const example =
        \\[.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}
        \\[...#.] (0,2,3,4) (2,3) (0,4) (0,1,2) (1,2,3,4) {7,5,12,7,2}
        \\[.###.#] (0,1,2,3,4) (0,3,4) (0,1,2,4,5) (1,2) {10,11,11,5,10,5}
    ;
    try std.testing.expectEqual(7, try part1(ally, example));
    try std.testing.expectEqual(33, try part2(ally, example));
}
