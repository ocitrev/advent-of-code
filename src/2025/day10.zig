const std = @import("std");
const utils = @import("utils");
const z3 = @cImport({
    @cInclude("z3.h");
});

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
    rawButtons: [][]u16,
    joltages: []u16,
    ally: std.mem.Allocator,

    fn parse(ally: std.mem.Allocator, line: []const u8) !Machine {
        var lights: u16 = 0;
        var buttonList = std.ArrayList(u16).empty;
        var rawButtonLists = std.ArrayList([]u16).empty;
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
                var rawButtonList = std.ArrayList(u16).empty;
                var buttons: u16 = 0;
                while (buttonIt.next()) |btnStr| {
                    const btn = try std.fmt.parseInt(u16, btnStr, 10);
                    buttons |= @as(u16, 1) << @intCast(btn);
                    try rawButtonList.append(ally, btn);
                }
                try buttonList.append(ally, buttons);
                try rawButtonLists.append(ally, try rawButtonList.toOwnedSlice(ally));
            } else if (value[0] == '{') {
                var joltageIt = std.mem.splitScalar(u8, value[1 .. value.len - 1], ',');
                while (joltageIt.next()) |j| {
                    try joltages.append(ally, try std.fmt.parseInt(u16, j, 10));
                }
            }
        }

        return Machine{
            .lights = lights,
            .buttons = try buttonList.toOwnedSlice(ally),
            .rawButtons = try rawButtonLists.toOwnedSlice(ally),
            .joltages = try joltages.toOwnedSlice(ally),
            .ally = ally,
        };
    }

    fn deinit(self: *@This()) void {
        for (self.rawButtons) |b| {
            self.ally.free(b);
        }

        self.ally.free(self.buttons);
        self.ally.free(self.rawButtons);
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

    fn configureJoltage(self: *const @This()) !Int {
        const cfg = z3.Z3_mk_config();
        defer z3.Z3_del_config(cfg);

        const ctx = z3.Z3_mk_context(cfg);
        defer z3.Z3_del_context(ctx);

        const opt = z3.Z3_mk_optimize(ctx);
        z3.Z3_optimize_inc_ref(ctx, opt);
        defer z3.Z3_optimize_dec_ref(ctx, opt);

        var values = try self.ally.alloc(z3.Z3_ast, self.rawButtons.len);
        defer self.ally.free(values);

        for (0..self.rawButtons.len) |i| {
            var buffer: [32]u8 = undefined;
            const name = try std.fmt.bufPrint(&buffer, "B{}", .{i});

            const sym = z3.Z3_mk_string_symbol(ctx, name.ptr);
            const int_sort = z3.Z3_mk_int_sort(ctx);
            const var_ast = z3.Z3_mk_const(ctx, sym, int_sort);
            values[i] = var_ast;

            const zero = z3.Z3_mk_int(ctx, 0, int_sort);
            const ge = z3.Z3_mk_ge(ctx, var_ast, zero);
            _ = z3.Z3_optimize_assert(ctx, opt, ge);
        }

        const int_sort = z3.Z3_mk_int_sort(ctx);
        for (self.joltages, 0..) |jolt, i| {
            var terms = std.ArrayList(z3.Z3_ast).empty;
            defer terms.deinit(self.ally);

            for (self.rawButtons, 0..) |bset, j| {
                for (bset) |x| {
                    if (x == i) {
                        try terms.append(self.ally, values[j]);
                        break;
                    }
                }
            }

            const sum_ast = if (terms.items.len == 0)
                z3.Z3_mk_int(ctx, 0, int_sort)
            else
                z3.Z3_mk_add(ctx, @intCast(terms.items.len), terms.items.ptr);

            const rhs = z3.Z3_mk_int64(ctx, jolt, int_sort);
            const eq = z3.Z3_mk_eq(ctx, sum_ast, rhs);
            _ = z3.Z3_optimize_assert(ctx, opt, eq);
        }

        const sum_total = z3.Z3_mk_add(ctx, @intCast(values.len), values.ptr);
        _ = z3.Z3_optimize_minimize(ctx, opt, sum_total);

        const result = z3.Z3_optimize_check(ctx, opt, 0, null);
        if (result != z3.Z3_L_TRUE)
            return error.Unsat;

        const model = z3.Z3_optimize_get_model(ctx, opt);
        z3.Z3_model_inc_ref(ctx, model);
        defer z3.Z3_model_dec_ref(ctx, model);

        var total: Int = 0;
        const num_consts = z3.Z3_model_get_num_consts(ctx, model);

        for (0..num_consts) |k| {
            const decl = z3.Z3_model_get_const_decl(ctx, model, @intCast(k));
            const val = z3.Z3_model_get_const_interp(ctx, model, decl);

            if (val != null) {
                var out: i64 = 0;
                if (z3.Z3_get_numeral_int64(ctx, val, &out)) {
                    total += @intCast(out);
                }
            }
        }

        return total;
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

    fn part2(self: *const @This()) !Int {
        var total: Int = 0;
        for (self.list) |machine| {
            total += try machine.configureJoltage();
        }
        return total;
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
    return try machines.part2();
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

    const example2 =
        \\[#.#####] (2,3,4,6) (2,5) (1,3,4,5,6) (1,2,5,6) (0,5,6) (0,1,2,3,4,6) (1,2,3,5,6) (1,3,4,6) (0,2,3,4,5,6) {23,42,62,53,35,62,74}
    ;
    try std.testing.expectEqual(1, try part1(ally, example2));
    try std.testing.expectEqual(74, try part2(ally, example2));
}
