const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/2024/day/9
    utils.printTitle(2024, 9, "Disk Fragmenter");

    const m = utils.Monitor.init();
    defer m.deinit();
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const ally = gpa.allocator();
    const input = comptime utils.trimInput(@embedFile("input"));

    const p1 = try part1(input, ally);
    utils.printAnswer(1, p1);
    std.debug.assert(6288707484810 == p1);

    const p2 = try part2(input, ally);
    utils.printAnswer(2, p2);
    std.debug.assert(6311837662089 == p2);
}

const Block = struct {
    start: usize,
    len: usize,
};

const Disk = struct {
    files: std.ArrayList(Block),
    freespaces: std.ArrayList(Block),
    data: std.ArrayList(?u32),
    nextFileId: u32,

    pub fn parse(input: []const u8, ally: std.mem.Allocator) !Disk {
        var files = std.ArrayList(Block).init(ally);
        var freespaces = std.ArrayList(Block).init(ally);
        var disk = std.ArrayList(?u32).init(ally);
        var nextId: u32 = 0;

        for (0.., input) |i, c| {
            const blockLen = try std.fmt.charToDigit(c, 10);
            if (i & 1 == 0) {
                std.debug.assert(blockLen != 0);
                try files.append(.{ .start = disk.items.len, .len = blockLen });
                try disk.appendNTimes(nextId, blockLen);
                nextId += 1;
            } else if (blockLen != 0) {
                try freespaces.append(.{ .start = disk.items.len, .len = blockLen });
                try disk.appendNTimes(null, blockLen);
            }
        }

        return .{
            .files = files,
            .freespaces = freespaces,
            .data = disk,
            .nextFileId = nextId,
        };
    }

    pub fn deinit(self: *@This()) void {
        self.files.deinit();
        self.freespaces.deinit();
        self.data.deinit();
    }

    pub fn checksum(self: *const @This()) u64 {
        var result: u64 = 0;
        for (0.., self.data.items) |i, n| {
            if (n) |num| {
                result += i * num;
            }
        }

        return result;
    }
};

fn part1(input: []const u8, ally: std.mem.Allocator) !u64 {
    var disk = try Disk.parse(input, ally);
    defer disk.deinit();

    var f: usize = 0;
    var r = disk.data.items.len;
    while (r > f) {
        r -= 1;

        if (disk.data.items[r] == null) {
            continue;
        }

        while (f < disk.data.items.len and disk.data.items[f] != null) {
            f += 1;
        }

        if (r > f) {
            std.mem.swap(?u32, &disk.data.items[f], &disk.data.items[r]);
            f += 1;
        }
    }

    return disk.checksum();
}

test "part 1" {
    const example =
        \\2333133121414131402
    ;
    try std.testing.expectEqual(1928, try part1(example, std.testing.allocator));
}

fn part2(input: []const u8, ally: std.mem.Allocator) !u64 {
    var disk = try Disk.parse(input, ally);
    defer disk.deinit();
    var nextId = disk.nextFileId;

    while (nextId != 0) {
        nextId -= 1;
        const fileBlock = disk.files.items[nextId];

        for (disk.freespaces.items) |*freeBlock| {
            if (freeBlock.start >= fileBlock.start) break;
            if (freeBlock.len >= fileBlock.len) {
                var f = freeBlock.start;
                var r = fileBlock.start;
                for (0..fileBlock.len) |_| {
                    std.mem.swap(?u32, &disk.data.items[f], &disk.data.items[r]);
                    f += 1;
                    r += 1;
                }
                freeBlock.len -= fileBlock.len;
                freeBlock.start += fileBlock.len;
                break;
            }
        }
    }

    return disk.checksum();
}

test "part 2" {
    const example =
        \\2333133121414131402
    ;
    try std.testing.expectEqual(2858, try part2(example, std.testing.allocator));
}
