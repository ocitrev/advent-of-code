const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/2025/day/9
    utils.printTitle(2025, 9, "Movie Theater");

    const m = utils.Monitor.init();
    defer m.deinit();
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const ally = gpa.allocator();
    const input = comptime utils.trimInput(@embedFile("input"));

    const p1 = try part1(ally, input);
    utils.printAnswer(1, p1);
    std.debug.assert(4760959496 == p1);

    const p2 = try part2(ally, input);
    utils.printAnswer(2, p2);
    // std.debug.assert(0 == p2);
}

const Int = i64;
const P = utils.Point2d(Int);

const Grid = struct {
    points: std.ArrayList(P),
    ally: std.mem.Allocator,

    fn parse(ally: std.mem.Allocator, input: []const u8) !Grid {
        var points = std.ArrayList(P).empty;

        var lineIt = std.mem.tokenizeAny(u8, input, "\r\n");
        while (lineIt.next()) |line| {
            var it = std.mem.splitScalar(u8, line, ',');
            const x = std.fmt.parseInt(Int, it.next().?, 10) catch @panic("Invalid input");
            const y = std.fmt.parseInt(Int, it.next().?, 10) catch @panic("Invalid input");

            const p = P{ .x = x, .y = y };
            try points.append(ally, p);
        }

        return Grid{
            .points = points,
            .ally = ally,
        };
    }

    fn deinit(self: *@This()) void {
        self.points.deinit(self.ally);
    }

    fn area(p1: P, p2: P) Int {
        const w = @max(p1.x, p2.x) - @min(p1.x, p2.x) + 1;
        const h = @max(p1.y, p2.y) - @min(p1.y, p2.y) + 1;
        return w * h;
    }

    fn pointOnSegment(p: P, a: P, b: P) bool {
        // Check if p is collinear with a and b using cross product
        const cross = @as(i128, p.y - a.y) * @as(i128, b.x - a.x) -
            @as(i128, p.x - a.x) * @as(i128, b.y - a.y);

        if (cross != 0) return false;

        // Check if p is within the bounding box of the segment
        const min_x = @min(a.x, b.x);
        const max_x = @max(a.x, b.x);
        const min_y = @min(a.y, b.y);
        const max_y = @max(a.y, b.y);

        return p.x >= min_x and p.x <= max_x and p.y >= min_y and p.y <= max_y;
    }

    pub fn pointInside(self: *const @This(), p: P) bool {
        const vertices = self.points.items;
        if (vertices.len < 3) return false;

        var inside = false;
        var j = vertices.len - 1;

        for (vertices, 0..) |_, i| {
            const vi = vertices[i];
            const vj = vertices[j];

            // Check if point is on the edge
            if (pointOnSegment(p, vi, vj)) {
                return true;
            }

            // Check if point is on the edge's y-range
            if ((vi.y > p.y) != (vj.y > p.y)) {
                // Calculate x-coordinate of intersection
                const slope = @as(i128, vj.x - vi.x) * @as(i128, p.y - vi.y);
                const dy = @as(i128, vj.y - vi.y);
                const intersect_x = vi.x + @divFloor(slope, dy);

                if (p.x < intersect_x) {
                    inside = !inside;
                }
            }
            j = i;
        }

        return inside;
    }

    fn isValidRecangle(self: *const @This(), p1: P, p2: P) bool {
        const topLeft = P{ .x = @min(p1.x, p2.x), .y = @min(p1.y, p2.y) };
        const topRight = P{ .x = @max(p1.x, p2.x), .y = @min(p1.y, p2.y) };
        const bottomLeft = P{ .x = @min(p1.x, p2.x), .y = @max(p1.y, p2.y) };
        const bottomRight = P{ .x = @max(p1.x, p2.x), .y = @max(p1.y, p2.y) };

        var validTL = topLeft.eql(p1) or topLeft.eql(p2);
        var validTR = topRight.eql(p1) or topRight.eql(p2);
        var validBL = bottomLeft.eql(p1) or bottomLeft.eql(p2);
        var validBR = bottomRight.eql(p1) or bottomRight.eql(p2);

        if (!validTL and self.pointInside(topLeft)) {
            validTL = true;
        }
        if (!validTR and self.pointInside(topRight)) {
            validTR = true;
        }
        if (!validBL and self.pointInside(bottomLeft)) {
            validBL = true;
        }

        if (!validBR and self.pointInside(bottomRight)) {
            validBR = true;
        }

        return validTL and validTR and validBL and validBR;
    }

    fn getLargestRectangleArea(self: *const @This(), comptime checkInside: bool) Int {
        var largestArea: Int = 0;
        for (0.., self.points.items) |i, p1| {
            for (self.points.items[i + 1 ..]) |p2| {
                if (!checkInside or self.isValidRecangle(p1, p2)) {
                    largestArea = @max(area(p1, p2), largestArea);
                }
            }
        }

        return largestArea;
    }
};

fn part1(ally: std.mem.Allocator, input: []const u8) !Int {
    var grid = try Grid.parse(ally, input);
    defer grid.deinit();
    return grid.getLargestRectangleArea(false);
}

fn part2(ally: std.mem.Allocator, input: []const u8) !Int {
    var grid = try Grid.parse(ally, input);
    defer grid.deinit();
    return grid.getLargestRectangleArea(true);
}

test "parts 1,2" {
    const ally = std.testing.allocator;
    const example =
        \\7,1
        \\11,1
        \\11,7
        \\9,7
        \\9,5
        \\2,5
        \\2,3
        \\7,3
    ;

    var grid = try Grid.parse(ally, example);
    defer grid.deinit();

    try std.testing.expectEqual(true, grid.pointInside(.{ .x = 9, .y = 5 }));
    try std.testing.expectEqual(true, grid.pointInside(.{ .x = 2, .y = 3 }));
    try std.testing.expectEqual(true, grid.pointInside(.{ .x = 9, .y = 3 }));
    try std.testing.expectEqual(true, grid.pointInside(.{ .x = 2, .y = 5 }));

    try std.testing.expectEqual(50, try part1(ally, example));
    try std.testing.expectEqual(24, try part2(ally, example));
}

// tried: 4760959496 -> too high
// tried: 4618516475 -> too high
