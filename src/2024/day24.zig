const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/2024/day/24
    utils.printTitle(2024, 24, "Crossed Wires");

    const m = utils.Monitor.init();
    defer m.deinit();
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const ally = gpa.allocator();
    const input = comptime utils.trim_input(@embedFile("input"));

    const p1 = try part1(ally, input);
    utils.printAnswer(1, p1);
    std.debug.assert(51837135476040 == p1);

    const p2 = try part2(ally, input);
    defer ally.free(p2);
    utils.printAnswer(2, p2);
    std.debug.assert(std.mem.eql(u8, "hjf,kdh,kpp,sgj,vss,z14,z31,z35", p2));
}

const Int = u64;
const String = []const u8;

const Operation = enum {
    And,
    Or,
    Xor,
    Value,
};

const Node = struct {
    name: String,
    op: Operation = .Value,
    value: Int = 0,
    a: ?*Node = null,
    b: ?*Node = null,
    nbUsed: usize = 0,

    fn eval(self: *const @This()) Int {
        return switch (self.op) {
            .And => self.a.?.eval() & self.b.?.eval(),
            .Or => self.a.?.eval() | self.b.?.eval(),
            .Xor => self.a.?.eval() ^ self.b.?.eval(),
            .Value => self.value,
        };
    }

    fn isBadNode(self: *const @This(), nbZ: usize) bool {
        if (self.op == .Value) {
            return false;
        }

        if (self.name[0] == 'z') {
            if (self.bitIndex() < nbZ - 1) {
                return self.op != .Xor;
            }

            return false;
        }

        if (self.op == .Xor) {
            if (self.a.?.op == .Value and self.b.?.op == .Value) {
                return self.nbUsed != 2;
            }

            if (self.a.?.op != .Value or self.b.?.op != .Value) {
                return true;
            }
        }

        if (self.op == .And and self.a.?.op == .Value and self.b.?.op == .Value) {
            if (self.a.?.bitIndex() == 0) {
                return self.nbUsed != 2;
            }
            return self.nbUsed != 1;
        }

        return false;
    }

    fn bitIndex(self: *const @This()) Int {
        return std.fmt.parseInt(Int, self.name[1..], 10) catch unreachable;
    }

    fn intValue(self: *const @This()) Int {
        return self.eval() << @intCast(self.bitIndex());
    }
};

const Puzzle = struct {
    ally: std.mem.Allocator,
    nodes: std.ArrayList(*Node),
    map: std.StringHashMap(*Node),
    allZ: std.ArrayList(*Node),

    fn addNode(self: *@This(), name: String) !*Node {
        const node = try self.ally.create(Node);
        node.* = Node{ .name = name };
        try self.nodes.append(node);
        try self.map.putNoClobber(name, node);

        if (name[0] == 'z') {
            try self.allZ.append(node);
        }

        return node;
    }

    fn init(ally: std.mem.Allocator, input: []const u8) !Puzzle {
        var puzzle = Puzzle{
            .ally = ally,
            .nodes = std.ArrayList(*Node).init(ally),
            .map = std.StringHashMap(*Node).init(ally),
            .allZ = std.ArrayList(*Node).init(ally),
        };

        var parsingWires = true;
        var lineIt = std.mem.splitAny(u8, input, "\r\n");
        while (lineIt.next()) |line| {
            if (parsingWires) {
                if (line.len == 0) {
                    parsingWires = false;
                    continue;
                }

                var parts = std.mem.splitSequence(u8, line, ": ");
                const wire = parts.next().?;
                const value = parts.next().?;
                var n = try puzzle.addNode(wire);
                n.value = try std.fmt.charToDigit(value[0], 2);
                n.op = .Value;
            } else {
                var tokens = std.mem.tokenizeScalar(u8, line, ' ');
                const nameA = tokens.next().?;
                const opText = tokens.next().?;
                const nameB = tokens.next().?;
                _ = tokens.next();
                const outName = tokens.next().?;

                var a = puzzle.map.get(nameA) orelse try puzzle.addNode(nameA);
                var b = puzzle.map.get(nameB) orelse try puzzle.addNode(nameB);
                var out = puzzle.map.get(outName) orelse try puzzle.addNode(outName);

                if (std.mem.eql(u8, opText, "OR")) {
                    out.op = .Or;
                } else if (std.mem.eql(u8, opText, "AND")) {
                    out.op = .And;
                } else if (std.mem.eql(u8, opText, "XOR")) {
                    out.op = .Xor;
                } else {
                    std.debug.panic("Invalid operation: {s}", .{opText});
                }

                out.a = a;
                out.b = b;
                a.nbUsed += 1;
                b.nbUsed += 1;
            }
        }

        return puzzle;
    }

    fn deinit(self: *@This()) void {
        for (self.nodes.items) |node| {
            self.ally.destroy(node);
        }

        self.nodes.deinit();
        self.map.deinit();
        self.allZ.deinit();
    }

    fn resolve(self: *@This()) !Int {
        var value: Int = 0;
        value += 0;
        for (self.allZ.items) |node| {
            value |= node.intValue();
        }

        return value;
    }
};

fn part1(ally: std.mem.Allocator, input: []const u8) !Int {
    var puzzle = try Puzzle.init(ally, input);
    defer puzzle.deinit();
    return try puzzle.resolve();
}

fn stringLess(_: void, a: String, b: String) bool {
    return std.mem.order(u8, a, b) == .lt;
}

fn part2(ally: std.mem.Allocator, input: []const u8) !String {
    var puzzle = try Puzzle.init(ally, input);
    defer puzzle.deinit();

    var badNodes = std.ArrayList(String).init(ally);
    defer badNodes.deinit();

    for (puzzle.nodes.items) |node| {
        if (node.isBadNode(puzzle.allZ.items.len)) {
            try badNodes.append(node.name);
        }
    }

    std.mem.sort(String, badNodes.items, {}, stringLess);
    return try std.mem.join(ally, ",", badNodes.items);
}

test "part 1" {
    const ally = std.testing.allocator;
    const example =
        \\x00: 1
        \\x01: 1
        \\x02: 1
        \\y00: 0
        \\y01: 1
        \\y02: 0
        \\
        \\x00 AND y00 -> z00
        \\x01 XOR y01 -> z01
        \\x02 OR y02 -> z02
    ;
    const larger =
        \\x00: 1
        \\x01: 0
        \\x02: 1
        \\x03: 1
        \\x04: 0
        \\y00: 1
        \\y01: 1
        \\y02: 1
        \\y03: 1
        \\y04: 1
        \\
        \\ntg XOR fgs -> mjb
        \\y02 OR x01 -> tnw
        \\kwq OR kpj -> z05
        \\x00 OR x03 -> fst
        \\tgd XOR rvg -> z01
        \\vdt OR tnw -> bfw
        \\bfw AND frj -> z10
        \\ffh OR nrd -> bqk
        \\y00 AND y03 -> djm
        \\y03 OR y00 -> psh
        \\bqk OR frj -> z08
        \\tnw OR fst -> frj
        \\gnj AND tgd -> z11
        \\bfw XOR mjb -> z00
        \\x03 OR x00 -> vdt
        \\gnj AND wpb -> z02
        \\x04 AND y00 -> kjc
        \\djm OR pbm -> qhw
        \\nrd AND vdt -> hwm
        \\kjc AND fst -> rvg
        \\y04 OR y02 -> fgs
        \\y01 AND x02 -> pbm
        \\ntg OR kjc -> kwq
        \\psh XOR fgs -> tgd
        \\qhw XOR tgd -> z09
        \\pbm OR djm -> kpj
        \\x03 XOR y03 -> ffh
        \\x00 XOR y04 -> ntg
        \\bfw OR bqk -> z06
        \\nrd XOR fgs -> wpb
        \\frj XOR qhw -> z04
        \\bqk OR frj -> z07
        \\y03 OR x01 -> nrd
        \\hwm AND bqk -> z03
        \\tgd XOR rvg -> z12
        \\tnw OR pbm -> gnj
    ;
    try std.testing.expectEqual(4, try part1(ally, example));
    try std.testing.expectEqual(2024, try part1(ally, larger));
}
