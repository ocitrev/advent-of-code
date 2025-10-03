const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/2024/day/18
    utils.printTitle(2024, 18, "RAM Run");

    const m = utils.Monitor.init();
    defer m.deinit();
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const ally = gpa.allocator();
    const input = comptime utils.trimInput(@embedFile("input"));

    const size = Point2d{ .x = 71, .y = 71 };
    const nbBlocks = 1024;

    const p1 = try part1(ally, input, size, nbBlocks);
    utils.printAnswer(1, p1);
    std.debug.assert(380 == p1);

    const p2 = try part2(ally, input, size, nbBlocks);
    defer ally.free(p2);
    utils.printAnswer(2, p2);
    std.debug.assert(std.mem.eql(u8, "26,50", p2));
}

const Int = i32;
const Point2d = utils.Point2d(Int);

const Grid = struct {
    ally: std.mem.Allocator,
    size: Point2d,
    blocks: []Point2d,

    fn init(ally: std.mem.Allocator, input: []const u8, size: Point2d) !Grid {
        var blocks = std.ArrayList(Point2d).empty;

        var lineIt = std.mem.tokenizeAny(u8, input, "\r\n");
        while (lineIt.next()) |line| {
            var it = std.mem.splitScalar(u8, line, ',');
            const x = try std.fmt.parseInt(Int, it.next().?, 10);
            const y = try std.fmt.parseInt(Int, it.next().?, 10);
            try blocks.append(ally, Point2d{ .x = x, .y = y });
        }

        return .{
            .ally = ally,
            .size = size,
            .blocks = try blocks.toOwnedSlice(ally),
        };
    }

    fn deinit(self: *@This()) void {
        self.ally.free(self.blocks);
    }

    fn shortestPath(self: *@This(), nbBlocks: usize) ?Int {
        const start = Point2d{ .x = 0, .y = 0 };
        const goal = self.size.sub(1, 1);

        const Node = struct {
            pos: Point2d,
            dist: Int,
        };

        var q = std.ArrayList(Node).empty;
        defer q.deinit(self.ally);

        var visited = std.AutoHashMap(Point2d, void).init(self.ally);
        defer visited.deinit();

        q.append(self.ally, .{ .pos = start, .dist = 0 }) catch unreachable;

        while (q.items.len != 0) {
            const node = q.orderedRemove(0);

            if (node.pos.eql(goal)) {
                return node.dist;
            }

            if (visited.fetchPut(node.pos, {}) catch unreachable != null) {
                continue;
            }

            const dirs = [_]Point2d{
                node.pos.north(),
                node.pos.south(),
                node.pos.east(),
                node.pos.west(),
            };

            for (dirs) |newPos| {
                if (newPos.x < 0 or newPos.y < 0) continue;
                if (newPos.x >= self.size.x or newPos.y >= self.size.y) continue;
                if (self.contains(nbBlocks, newPos)) continue;
                q.append(self.ally, .{ .pos = newPos, .dist = node.dist + 1 }) catch unreachable;
            }
        }

        return null;
    }

    fn contains(self: *const @This(), nbBlocks: usize, p: Point2d) bool {
        for (self.blocks[0..nbBlocks]) |pp| {
            if (pp.eql(p)) return true;
        }

        return false;
    }
};

fn part1(ally: std.mem.Allocator, input: []const u8, size: Point2d, nbBlocks: usize) !Int {
    var grid = try Grid.init(ally, input, size);
    defer grid.deinit();
    return grid.shortestPath(nbBlocks).?;
}

fn part2(ally: std.mem.Allocator, input: []const u8, size: Point2d, nbBlocks: usize) ![]u8 {
    var grid = try Grid.init(ally, input, size);
    defer grid.deinit();

    var minBlocks = nbBlocks;
    var maxBlocks = grid.blocks.len;

    while (true) {
        const midBlocks = (minBlocks + maxBlocks) / 2;

        if (grid.shortestPath(midBlocks)) |_| {
            minBlocks = midBlocks;
        } else {
            maxBlocks = midBlocks;
        }

        if (maxBlocks - minBlocks <= 1) {
            const blocking = grid.blocks[minBlocks];
            return std.fmt.allocPrint(ally, "{},{}", .{ blocking.x, blocking.y });
        }
    }

    unreachable;
}

test "parts 1,2" {
    const ally = std.testing.allocator;
    const example =
        \\5,4
        \\4,2
        \\4,5
        \\3,0
        \\2,1
        \\6,3
        \\2,4
        \\1,5
        \\0,6
        \\3,3
        \\2,6
        \\5,1
        \\1,2
        \\5,5
        \\2,5
        \\6,5
        \\1,4
        \\0,4
        \\6,4
        \\1,1
        \\6,1
        \\1,0
        \\0,5
        \\1,6
        \\2,0
    ;
    const size = Point2d{ .x = 7, .y = 7 };
    const nbBlocks = 12;
    try std.testing.expectEqual(22, try part1(ally, example, size, nbBlocks));
    const p2 = try part2(ally, example, size, nbBlocks);
    defer ally.free(p2);
    try std.testing.expectEqualStrings("6,1", p2);
}
