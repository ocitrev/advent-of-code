const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/2024/day/17
    utils.printTitle(2024, 17, "Chronospatial Computer");

    const m = utils.Monitor.init();
    defer m.deinit();
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const ally = gpa.allocator();
    const input = comptime utils.trimInput(@embedFile("input"));

    const p1 = try part1(input, ally);
    defer ally.free(p1);
    utils.printAnswer(1, p1);
    std.debug.assert(std.mem.eql(u8, "6,2,7,2,3,1,6,0,5", p1));

    const p2 = try part2(input, ally);
    utils.printAnswer(2, p2);
    std.debug.assert(236548287712877 == p2);
}

const Int = u64;

const Computer = struct {
    ally: std.mem.Allocator,
    a: Int,
    b: Int,
    c: Int,
    ip: usize = 0,
    memory: []Int,
    output: std.array_list.Managed(Int),

    fn init(input: []const u8, ally: std.mem.Allocator) !@This() {
        var a: Int = undefined;
        var b: Int = undefined;
        var c: Int = undefined;

        var memory = std.array_list.Managed(Int).init(ally);
        defer memory.deinit();

        var lineIt = std.mem.tokenizeAny(u8, input, "\r\n");
        while (lineIt.next()) |line| {
            const registerTagLen = "Register X: ".len;
            const programTagLen = "Program: ".len;
            if (std.mem.startsWith(u8, line, "Register A: ")) {
                a = try std.fmt.parseInt(Int, line[registerTagLen..], 10);
            } else if (std.mem.startsWith(u8, line, "Register B: ")) {
                b = try std.fmt.parseInt(Int, line[registerTagLen..], 10);
            } else if (std.mem.startsWith(u8, line, "Register C: ")) {
                c = try std.fmt.parseInt(Int, line[registerTagLen..], 10);
            } else if (std.mem.startsWith(u8, line, "Program: ")) {
                var programIt = std.mem.tokenizeScalar(u8, line[programTagLen..], ',');
                while (programIt.next()) |opcode| {
                    const value = try std.fmt.parseInt(Int, opcode, 10);
                    try memory.append(value);
                }
            }
        }

        return .{
            .ally = ally,
            .memory = try memory.toOwnedSlice(),
            .a = a,
            .b = b,
            .c = c,
            .output = std.array_list.Managed(Int).init(ally),
        };
    }

    fn deinit(self: *@This()) void {
        self.ally.free(self.memory);
        self.output.deinit();
    }

    fn getComboValue(self: *@This()) Int {
        const op = self.read();
        return switch (op) {
            0...3 => op,
            4 => self.a,
            5 => self.b,
            6 => self.c,
            else => unreachable,
        };
    }

    fn adv(self: *@This()) void {
        self.a = self.a >> @intCast(self.getComboValue());
    }

    fn bxl(self: *@This()) void {
        self.b ^= self.read();
    }

    fn bst(self: *@This()) void {
        self.b = self.getComboValue() & 0b111;
    }

    fn jnz(self: *@This()) void {
        if (self.a == 0) {
            self.ip += 1;
            return;
        }

        self.ip = @intCast(self.memory[self.ip]);
    }

    fn bxc(self: *@This()) void {
        self.ip += 1;
        self.b ^= self.c;
    }

    fn out(self: *@This()) void {
        const v = self.getComboValue() & 0b111;
        self.output.append(v) catch unreachable;
    }

    fn bdv(self: *@This()) void {
        self.b = self.a >> @intCast(self.getComboValue());
    }

    fn cdv(self: *@This()) void {
        self.c = self.a >> @intCast(self.getComboValue());
    }

    fn read(self: *@This()) Int {
        defer self.ip += 1;
        return self.memory[self.ip];
    }

    fn run(self: *@This()) void {
        while (self.ip < self.memory.len) {
            switch (self.read()) {
                0 => self.adv(),
                1 => self.bxl(),
                2 => self.bst(),
                3 => self.jnz(),
                4 => self.bxc(),
                5 => self.out(),
                6 => self.bdv(),
                7 => self.cdv(),
                else => unreachable,
            }
        }
    }

    fn joinOutput(self: *const @This()) ![]u8 {
        var buffer = std.array_list.Managed(u8).init(self.ally);
        defer buffer.deinit();
        var first = true;
        const w = buffer.writer();
        for (self.output.items) |v| {
            if (first) {
                first = false;
            } else {
                try buffer.append(',');
            }
            try std.fmt.format(w, "{}", .{v});
        }

        return try buffer.toOwnedSlice();
    }

    fn debug(self: *const @This()) void {
        std.debug.print("a: {}, b:{}, c:{}, out: {any}\n", .{ self.a, self.b, self.c, self.output.items });
    }

    fn reset(self: *@This(), a: Int, b: Int, c: Int) void {
        self.a = a;
        self.b = b;
        self.c = c;
        self.ip = 0;
        self.output.clearRetainingCapacity();
    }

    fn findA(self: *@This()) Int {
        return self.findARecurse(0, self.b, self.c, self.memory.len).?;
    }

    fn findARecurse(self: *@This(), a: Int, b: Int, c: Int, index: usize) ?Int {
        if (index == 0) {
            return a;
        }

        for (0..8) |i| {
            const testA = (a << 3) + i;
            self.reset(testA, b, c);
            self.run();
            const first = self.output.items[0];
            const last = index - 1;
            if (first == self.memory[last]) {
                if (self.findARecurse(testA, b, c, last)) |result| {
                    return result;
                }
            }
        }

        return null;
    }
};

fn part1(input: []const u8, ally: std.mem.Allocator) ![]u8 {
    var cpu = try Computer.init(input, ally);
    defer cpu.deinit();
    cpu.run();
    return cpu.joinOutput();
}

fn part2(input: []const u8, ally: std.mem.Allocator) !Int {
    var cpu = try Computer.init(input, ally);
    defer cpu.deinit();
    return cpu.findA();
}

test "example 1" {
    const example =
        \\Register A: 0
        \\Register B: 0
        \\Register C: 9
        \\
        \\Program: 2,6
    ;
    var cpu = try Computer.init(example, std.testing.allocator);
    defer cpu.deinit();
    cpu.run();
    try std.testing.expectEqual(1, cpu.b);
}

test "example 2" {
    const example =
        \\Register A: 10
        \\Register B: 0
        \\Register C: 0
        \\
        \\Program: 5,0,5,1,5,4
    ;
    var cpu = try Computer.init(example, std.testing.allocator);
    defer cpu.deinit();
    cpu.run();

    const result = try cpu.joinOutput();
    defer std.testing.allocator.free(result);
    try std.testing.expectEqualStrings("0,1,2", result);
}

test "example 3" {
    const example =
        \\Register A: 2024
        \\Register B: 0
        \\Register C: 0
        \\
        \\Program: 0,1,5,4,3,0
    ;
    var cpu = try Computer.init(example, std.testing.allocator);
    defer cpu.deinit();
    cpu.run();

    try std.testing.expectEqual(0, cpu.a);
    const result = try cpu.joinOutput();
    defer std.testing.allocator.free(result);
    try std.testing.expectEqualStrings("4,2,5,6,7,7,7,7,3,1,0", result);
}

test "example 4" {
    const example =
        \\Register A: 0
        \\Register B: 29
        \\Register C: 0
        \\
        \\Program: 1,7
    ;
    var cpu = try Computer.init(example, std.testing.allocator);
    defer cpu.deinit();
    cpu.run();
    try std.testing.expectEqual(26, cpu.b);
}

test "example 5" {
    const example =
        \\Register A: 0
        \\Register B: 2024
        \\Register C: 43690
        \\
        \\Program: 4,0
    ;
    var cpu = try Computer.init(example, std.testing.allocator);
    defer cpu.deinit();
    cpu.run();
    try std.testing.expectEqual(44354, cpu.b);
}

test "part 1" {
    const example =
        \\Register A: 729
        \\Register B: 0
        \\Register C: 0
        \\
        \\Program: 0,1,5,4,3,0
    ;
    const p1 = try part1(example, std.testing.allocator);
    defer std.testing.allocator.free(p1);
    try std.testing.expectEqualStrings("4,6,3,5,6,3,5,2,1,0", p1);
}

test "part 2" {
    const example =
        \\Register A: 2024
        \\Register B: 0
        \\Register C: 0
        \\
        \\Program: 0,3,5,4,3,0
    ;

    const p2 = try part2(example, std.testing.allocator);
    try std.testing.expectEqual(117440, p2);
}
