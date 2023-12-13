const std = @import("std");
const Cache = std.AutoHashMap(struct { u8, u8, u8 }, u64);

fn countArrangementsRec(line: []const u8, groups: []const usize, cache: *Cache, i: u8, gi: u8, count: u8) !u64 {
    const key = .{ i, gi, count };
    if (cache.get(key)) |ans| {
        return ans;
    }

    if (i == line.len) {
        if (gi == groups.len and count == 0) {
            return 1;
        } else if (gi == groups.len - 1 and groups[gi] == count) {
            return 1;
        } else {
            return 0;
        }
    }

    var ans: u64 = 0;
    for ([_]u8{ '.', '#' }) |c| {
        if (line[i] == c or line[i] == '?') {
            if (c == '.' and count == 0) {
                ans += try countArrangementsRec(line, groups, cache, i + 1, gi, 0);
            } else if (c == '.' and count != 0 and gi < groups.len and groups[gi] == count) {
                ans += try countArrangementsRec(line, groups, cache, i + 1, gi + 1, 0);
            } else if (c == '#') {
                ans += try countArrangementsRec(line, groups, cache, i + 1, gi, count + 1);
            }
        }
    }

    try cache.put(key, ans);
    return ans;
}

fn countArrangements(nb_copy: u8, line: []const u8, allocator: std.mem.Allocator) !u64 {
    var cache = Cache.init(allocator);
    defer cache.deinit();

    const sep = std.mem.indexOfScalar(u8, line, ' ').?;
    var groups = std.ArrayList(usize).init(allocator);
    defer groups.deinit();

    var lines = std.ArrayList([]const u8).init(allocator);
    defer lines.deinit();

    for (0..nb_copy) |_| {
        var it = std.mem.tokenizeScalar(u8, line[sep + 1 ..], ',');
        while (it.next()) |num| {
            try groups.append(try std.fmt.parseInt(usize, num, 10));
        }
        try lines.append(line[0..sep]);
    }

    var data = try std.mem.join(allocator, "?", lines.items);
    defer allocator.free(data);

    return try countArrangementsRec(data, groups.items, &cache, 0, 0, 0);
}

test "countArrangements" {
    try std.testing.expectEqual(@as(u64, 1), try countArrangements(1, "???.### 1,1,3", std.testing.allocator));
    try std.testing.expectEqual(@as(u64, 4), try countArrangements(1, ".??..??...?##. 1,1,3", std.testing.allocator));
    try std.testing.expectEqual(@as(u64, 1), try countArrangements(1, "?#?#?#?#?#?#?#? 1,3,1,6", std.testing.allocator));
    try std.testing.expectEqual(@as(u64, 1), try countArrangements(1, "????.#...#... 4,1,1", std.testing.allocator));
    try std.testing.expectEqual(@as(u64, 4), try countArrangements(1, "????.######..#####. 1,6,5", std.testing.allocator));
    try std.testing.expectEqual(@as(u64, 10), try countArrangements(1, "?###???????? 3,2,1", std.testing.allocator));
}

fn part1(input: []const u8, allocator: std.mem.Allocator) !u64 {
    var total: u64 = 0;
    var it = std.mem.tokenizeAny(u8, input, "\r\n");

    while (it.next()) |line| {
        total += try countArrangements(1, line, allocator);
    }

    return total;
}

test "part 1" {
    const example =
        \\???.### 1,1,3
        \\.??..??...?##. 1,1,3
        \\?#?#?#?#?#?#?#? 1,3,1,6
        \\????.#...#... 4,1,1
        \\????.######..#####. 1,6,5
        \\?###???????? 3,2,1
    ;
    try std.testing.expectEqual(@as(u64, 21), try part1(example, std.testing.allocator));
}

fn part2(input: []const u8, allocator: std.mem.Allocator) !u64 {
    var total: u64 = 0;
    var it = std.mem.tokenizeAny(u8, input, "\r\n");

    while (it.next()) |line| {
        total += try countArrangements(5, line, allocator);
    }

    return total;
}

test "countArrangements 5" {
    try std.testing.expectEqual(@as(u64, 1), try countArrangements(5, "???.### 1,1,3", std.testing.allocator));
    try std.testing.expectEqual(@as(u64, 16384), try countArrangements(5, ".??..??...?##. 1,1,3", std.testing.allocator));
    try std.testing.expectEqual(@as(u64, 1), try countArrangements(5, "?#?#?#?#?#?#?#? 1,3,1,6", std.testing.allocator));
    try std.testing.expectEqual(@as(u64, 16), try countArrangements(5, "????.#...#... 4,1,1", std.testing.allocator));
    try std.testing.expectEqual(@as(u64, 2500), try countArrangements(5, "????.######..#####. 1,6,5", std.testing.allocator));
    try std.testing.expectEqual(@as(u64, 506250), try countArrangements(5, "?###???????? 3,2,1", std.testing.allocator));
}

test "part 2" {
    const example =
        \\???.### 1,1,3
        \\.??..??...?##. 1,1,3
        \\?#?#?#?#?#?#?#? 1,3,1,6
        \\????.#...#... 4,1,1
        \\????.######..#####. 1,6,5
        \\?###???????? 3,2,1
    ;
    try std.testing.expectEqual(@as(u64, 525152), try part2(example, std.testing.allocator));
}

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();

    // https://adventofcode.com/2023/day/12
    const input = @embedFile("input");
    std.debug.print("Day 12, 2023: Hot Springs\n", .{});
    std.debug.print("  Part 1: {}\n", .{try part1(input, allocator)});
    std.debug.print("  Part 2: {}\n", .{try part2(input, allocator)});
}
