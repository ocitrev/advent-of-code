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
    std.debug.assert(1343576598 == p2);
}

const Int = i64;
const P = utils.Point2d(Int);

const Grid = struct {
    points: []P,
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
            .points = try points.toOwnedSlice(ally),
            .ally = ally,
        };
    }

    fn deinit(self: *@This()) void {
        self.ally.free(self.points);
    }

    fn area(p1: P, p2: P) Int {
        const w = @as(Int, @intCast(@abs(p1.x - p2.x))) + 1;
        const h = @as(Int, @intCast(@abs(p1.y - p2.y))) + 1;
        return w * h;
    }

    fn pointOnSegment(p: P, a: P, b: P) bool {
        if (a.x != b.x and a.y != b.y) {
            @panic("pointOnSegment only supports axis-aligned segments");
        }

        // Check if p is within the bounding box of the segment
        const min_x = @min(a.x, b.x);
        const max_x = @max(a.x, b.x);
        const min_y = @min(a.y, b.y);
        const max_y = @max(a.y, b.y);

        return p.x >= min_x and p.x <= max_x and p.y >= min_y and p.y <= max_y;
    }

    pub fn pointInside(self: *const @This(), p: P) bool {
        const vertices = self.points;
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

            if ((vi.y > p.y) != (vj.y > p.y)) {
                const slope = (vj.x - vi.x) * (p.y - vi.y);
                const dy = (vj.y - vi.y);
                const intersect_x = vi.x + @divFloor(slope, dy);

                if (p.x < intersect_x) {
                    inside = !inside;
                }
            }

            j = i;
        }

        return inside;
    }
    fn doesIntersect(self: *const @This(), p1: P, p2: P) bool {
        const vertices = self.points;
        var j = vertices.len - 1;

        if (p1.x == p2.x) {
            // Vertical segment from p1 to p2
            const segX = p1.x;
            const segTopY = @min(p1.y, p2.y);
            const segBottomY = @max(p1.y, p2.y);

            for (vertices, 0..) |_, i| {
                const v1 = vertices[i];
                const v2 = vertices[j];

                if (v1.y == v2.y) {
                    // Horizontal edge
                    const edgeY = v1.y;
                    const edgeLeftX = @min(v1.x, v2.x);
                    const edgeRightX = @max(v1.x, v2.x);

                    // Check if the vertical segment crosses this horizontal edge
                    if (edgeY > segTopY and edgeY < segBottomY and
                        segX > edgeLeftX and segX < edgeRightX)
                    {
                        return true;
                    }
                }

                j = i;
            }

            return false;
        } else if (p1.y == p2.y) {
            // Horizontal segment from p1 to p2
            const segY = p1.y;
            const segLeftX = @min(p1.x, p2.x);
            const segRightX = @max(p1.x, p2.x);

            for (vertices, 0..) |_, i| {
                const v1 = vertices[i];
                const v2 = vertices[j];

                if (v1.x == v2.x) {
                    // Vertical edge
                    const edgeX = v1.x;
                    const edgeTopY = @min(v1.y, v2.y);
                    const edgeBottomY = @max(v1.y, v2.y);

                    // Check if the horizontal segment crosses this vertical edge
                    if (edgeX > segLeftX and edgeX < segRightX and
                        segY > edgeTopY and segY < edgeBottomY)
                    {
                        return true;
                    }
                }

                j = i;
            }

            return false;
        }

        @panic("Sloped segments not supported");
    }

    fn isValidRecangle(self: *const @This(), p1: P, p2: P) bool {
        const left = @min(p1.x, p2.x);
        const right = @max(p1.x, p2.x);
        const top = @min(p1.y, p2.y);
        const bottom = @max(p1.y, p2.y);

        const corners = [_]P{
            .{ .x = left, .y = top },
            .{ .x = right, .y = top },
            .{ .x = left, .y = bottom },
            .{ .x = right, .y = bottom },
        };

        if (self.doesIntersect(corners[0], corners[1]) or
            self.doesIntersect(corners[2], corners[3]) or
            self.doesIntersect(corners[0], corners[2]) or
            self.doesIntersect(corners[1], corners[3]))
        {
            return false;
        }

        for (corners) |corner| {
            if (corner.eql(p1) or corner.eql(p2)) continue;
            if (!self.pointInside(corner)) {
                return false;
            }
        }

        return true;
    }

    fn getLargestRectangleArea(self: *const @This(), comptime checkInside: bool) Int {
        var largestArea: Int = 0;
        for (0.., self.points) |i, p1| {
            for (self.points[i + 1 ..]) |p2| {
                const a = area(p1, p2);
                if (a <= largestArea) continue;

                if (!checkInside or self.isValidRecangle(p1, p2)) {
                    largestArea = a;
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
