const std = @import("std");
const utils = @import("utils");
const Result = utils.Result(i32);

pub fn main() !void {
    // https://adventofcode.com/2024/day/5
    utils.printTitle(2024, 5, "Print Queue");

    const m = utils.Monitor.init();
    defer m.deinit();
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const ally = gpa.allocator();
    const input = comptime utils.trim_input(@embedFile("input"));

    const result = try run(input, ally);
    utils.printAnswer(1, result.part1);
    std.debug.assert(5091 == result.part1);
    utils.printAnswer(2, result.part2);
    std.debug.assert(4681 == result.part2);
}

const Queue = struct {
    rules: std.ArrayList([2]i32),
    queue: std.ArrayList(std.ArrayList(i32)),
    ally: std.mem.Allocator,

    pub fn init(input: []const u8, ally: std.mem.Allocator) !Queue {
        var queue = std.ArrayList(std.ArrayList(i32)).init(ally);
        var rules = std.ArrayList([2]i32).init(ally);

        var line_it = std.mem.splitAny(u8, input, "\r\n");
        var parsing_rules = true;
        while (line_it.next()) |line| {
            if (line.len == 0) {
                parsing_rules = false;
                continue;
            }

            if (parsing_rules) {
                var it = std.mem.splitScalar(u8, line, '|');
                const a = std.fmt.parseInt(i32, it.next().?, 10) catch unreachable;
                const b = std.fmt.parseInt(i32, it.next().?, 10) catch unreachable;
                try rules.append([_]i32{ a, b });
            } else {
                var items = std.ArrayList(i32).init(ally);
                var it = std.mem.splitScalar(u8, line, ',');
                while (it.next()) |item| {
                    try items.append(std.fmt.parseInt(i32, item, 10) catch unreachable);
                }

                try queue.append(items);
            }
        }

        return .{
            .queue = queue,
            .rules = rules,
            .ally = ally,
        };
    }

    fn sort_queue(self: *const @This(), a: i32, b: i32) bool {
        for (self.rules.items) |r| {
            if (a == r[0] and b == r[1]) {
                return true;
            } else if (b == r[0] and a == r[1]) {
                return false;
            }
        }
        return false;
    }

    pub fn sum_middle_pages(self: *const @This()) !Result {
        var sum_correct: i32 = 0;
        var sum_incorrect: i32 = 0;

        for (self.queue.items) |q| {
            const sorted = try self.ally.alloc(i32, q.items.len);
            defer self.ally.free(sorted);
            std.mem.copyForwards(i32, sorted, q.items);
            std.mem.sort(i32, sorted, self, sort_queue);

            if (std.mem.order(i32, q.items, sorted) == .eq) {
                sum_correct += q.items[q.items.len / 2];
            } else {
                sum_incorrect += sorted[sorted.len / 2];
            }
        }

        return .{
            .part1 = sum_correct,
            .part2 = sum_incorrect,
        };
    }

    pub fn deinit(self: *const @This()) void {
        self.rules.deinit();
        for (self.queue.items) |items| {
            items.deinit();
        }
        self.queue.deinit();
    }
};

fn run(input: []const u8, ally: std.mem.Allocator) !Result {
    const queue = try Queue.init(input, ally);
    defer queue.deinit();
    return try queue.sum_middle_pages();
}

test "part 1,2" {
    const example =
        \\47|53
        \\97|13
        \\97|61
        \\97|47
        \\75|29
        \\61|13
        \\75|53
        \\29|13
        \\97|29
        \\53|29
        \\61|53
        \\97|53
        \\61|29
        \\47|13
        \\75|47
        \\97|75
        \\47|61
        \\75|61
        \\47|29
        \\75|13
        \\53|13
        \\
        \\75,47,61,53,29
        \\97,61,53,29,13
        \\75,29,13
        \\75,97,47,61,53
        \\61,13,29
        \\97,13,75,29,47
    ;
    const result = try run(example, std.testing.allocator);
    try std.testing.expectEqual(143, result.part1);
    try std.testing.expectEqual(123, result.part2);
}
