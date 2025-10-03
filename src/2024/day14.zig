const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/2024/day/14
    utils.printTitle(2024, 14, "Restroom Redoubt");

    const m = utils.Monitor.init();
    defer m.deinit();
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const ally = gpa.allocator();
    const input = comptime utils.trimInput(@embedFile("input"));

    const size = Point2d{ .x = 101, .y = 103 };
    const p1 = try part1(input, size, ally);
    utils.printAnswer(1, p1);
    std.debug.assert(231221760 == p1);

    const p2 = try part2(input, size, ally);
    utils.printAnswer(2, p2);
    std.debug.assert(6771 == p2);
}

const Int = i32;
const Point2d = utils.Point2d(Int);
const Robot = struct {
    pos: Point2d,
    vel: Point2d,

    fn run(self: *@This(), times: Int, size: Point2d) void {
        const newPos = self.pos.addp(self.vel.mul(times));
        self.pos.x = @mod(newPos.x, size.x);
        self.pos.y = @mod(newPos.y, size.y);
    }
};

const Robots = struct {
    size: Point2d,
    robots: []Robot,
    buffer: []u8,
    elapsed: Int,
    ally: std.mem.Allocator,
    ansi: bool,
    robotSet: std.AutoHashMap(Point2d, void),

    fn parseRobots(input: []const u8, ally: std.mem.Allocator) ![]Robot {
        var robots = std.ArrayList(Robot).empty;
        defer robots.deinit(ally);
        var lineIt = std.mem.tokenizeAny(u8, input, "\r\n");

        while (lineIt.next()) |line| {
            var p = utils.Parser.init(line);
            _ = p.nextScalar('=');

            const pos = Point2d{
                .x = try std.fmt.parseInt(Int, p.nextScalar(',').?, 10),
                .y = try std.fmt.parseInt(Int, p.nextScalar(' ').?, 10),
            };

            _ = p.nextScalar('=');
            const vel = Point2d{
                .x = try std.fmt.parseInt(Int, p.nextScalar(',').?, 10),
                .y = try std.fmt.parseInt(Int, p.rest(), 10),
            };

            try robots.append(ally, .{ .pos = pos, .vel = vel });
        }

        return robots.toOwnedSlice(ally);
    }

    fn init(input: []const u8, size: Point2d, ally: std.mem.Allocator) !Robots {
        const robots = try parseRobots(input, ally);

        return .{
            .size = size,
            .robots = robots,
            .buffer = try ally.alloc(u8, @intCast((size.x + 2) * size.y + 2)),
            .elapsed = 0,
            .ally = ally,
            .ansi = std.fs.File.stdout().getOrEnableAnsiEscapeSupport(),
            .robotSet = std.AutoHashMap(Point2d, void).init(ally),
        };
    }

    fn deinit(self: *@This()) void {
        self.ally.free(self.robots);
        self.ally.free(self.buffer);
        self.robotSet.deinit();
    }

    fn advance(self: *@This(), stepCount: Int) void {
        self.elapsed += stepCount;
        for (self.robots) |*r| {
            r.run(stepCount, self.size);
        }
    }

    fn render(self: *@This()) void {
        if (!self.ansi) return;
        var b = std.ArrayList(u8).initBuffer(self.buffer);
        b.printAssumeCapacity("\x1b[1;1HElapsed: {}                   \n", .{self.elapsed});

        for (0..@intCast(self.size.y)) |y| {
            for (0..@intCast(self.size.x)) |x| {
                const p = Point2d{ .x = @intCast(x), .y = @intCast(y) };
                var nbRobots: Int = 0;
                for (self.robots) |r| {
                    if (r.pos.eql(p)) {
                        nbRobots += 1;
                        break;
                    }
                }

                if (nbRobots == 0) {
                    b.appendAssumeCapacity(' ');
                } else {
                    for ("█") |c| b.appendAssumeCapacity(c);
                }
            }

            b.appendAssumeCapacity('\n');
        }

        std.fs.File.stdout().writeAll(b.items) catch unreachable;
    }

    const Quadrant = enum {
        topLeft,
        topRight,
        bottomLeft,
        bottomRight,
    };

    const Quadrants = struct {
        topLeft: Int,
        topRight: Int,
        bottomLeft: Int,
        bottomRight: Int,

        fn safetyScore(self: *const @This()) Int {
            return self.topLeft * self.topRight * self.bottomLeft * self.bottomRight;
        }

        fn getHighest(self: *const @This()) ?Quadrant {
            if (self.topLeft > self.topRight + self.bottomLeft + self.bottomRight) {
                return .topLeft;
            } else if (self.topRight > self.topLeft + self.bottomLeft + self.bottomRight) {
                return .topRight;
            } else if (self.bottomLeft > self.topLeft + self.topRight + self.bottomRight) {
                return .bottomLeft;
            } else if (self.bottomRight > self.topLeft + self.topRight + self.bottomLeft) {
                return .bottomRight;
            } else {
                return null;
            }
        }
    };

    fn renderQuadrant(self: *const @This(), quadrant: Quadrant) void {
        if (!self.ansi) return;
        var b = std.ArrayList(u8).initBuffer(self.buffer);
        b.printAssumeCapacity("\x1b[1;1HElapsed: {}                   \n", .{self.elapsed});
        const halfW = @divTrunc(self.size.x, 2);
        const halfH = @divTrunc(self.size.y, 2);
        const offset = switch (quadrant) {
            .topLeft => Point2d{ .x = 0, .y = 0 },
            .topRight => Point2d{ .x = halfW, .y = 0 },
            .bottomLeft => Point2d{ .x = 0, .y = halfH },
            .bottomRight => Point2d{ .x = halfW, .y = halfH },
        };

        for (0..@intCast(halfH)) |y| {
            for (0..@intCast(halfW)) |x| {
                const p = offset.add(@intCast(x), @intCast(y));
                var nbRobots: Int = 0;
                for (self.robots) |r| {
                    if (r.pos.eql(p)) {
                        nbRobots += 1;
                        break;
                    }
                }

                if (nbRobots == 0) {
                    b.appendAssumeCapacity(' ');
                } else {
                    for ("█") |c| b.appendAssumeCapacity(c);
                }
            }

            b.appendAssumeCapacity('\n');
        }

        std.fs.File.stdout().writeAll(b.items) catch unreachable;
    }

    fn splitToQuadrants(self: *const @This()) Quadrants {
        const middle = Point2d{
            .x = @divTrunc(self.size.x, 2),
            .y = @divTrunc(self.size.y, 2),
        };

        var quadrants = Quadrants{ .topLeft = 0, .topRight = 0, .bottomLeft = 0, .bottomRight = 0 };
        for (self.robots) |*r| {
            if (r.pos.x < middle.x and r.pos.y < middle.y) {
                quadrants.topLeft += 1;
            } else if (r.pos.x > middle.x and r.pos.y < middle.y) {
                quadrants.topRight += 1;
            } else if (r.pos.x < middle.x and r.pos.y > middle.y) {
                quadrants.bottomLeft += 1;
            } else if (r.pos.x > middle.x and r.pos.y > middle.y) {
                quadrants.bottomRight += 1;
            }
        }

        return quadrants;
    }

    fn isEasterEgg(self: *@This()) ?Quadrant {
        return self.splitToQuadrants().getHighest();
    }
};

fn part1(input: []const u8, size: Point2d, ally: std.mem.Allocator) !Int {
    var robots = try Robots.init(input, size, ally);
    defer robots.deinit();
    robots.advance(100);
    return robots.splitToQuadrants().safetyScore();
}

fn part2(input: []const u8, size: Point2d, ally: std.mem.Allocator) !i32 {
    var robots = try Robots.init(input, size, ally);
    defer robots.deinit();
    const limit = size.x * size.y;

    while (robots.elapsed < limit) {
        robots.advance(1);

        if (robots.isEasterEgg()) |q| {
            if (robots.ansi) {
                std.debug.print("\x1b[?1049h\x1b[?25l", .{});
                robots.renderQuadrant(q);
                std.Thread.sleep(std.time.ns_per_ms * 250);
                std.debug.print("\x1b[?1049l\x1b[?25h", .{});
            }
            break;
        }
    }

    return robots.elapsed;
}

test "parts 1,2" {
    const example =
        \\p=0,4 v=3,-3
        \\p=6,3 v=-1,-3
        \\p=10,3 v=-1,2
        \\p=2,0 v=2,-1
        \\p=0,0 v=1,3
        \\p=3,0 v=-2,-2
        \\p=7,6 v=-1,-3
        \\p=3,0 v=-1,-2
        \\p=9,3 v=2,3
        \\p=7,3 v=-1,2
        \\p=2,4 v=2,-3
        \\p=9,5 v=-3,-3
    ;
    const size = Point2d{ .x = 11, .y = 7 };
    try std.testing.expectEqual(12, try part1(example, size, std.testing.allocator));
    try std.testing.expectEqual(6, try part2(example, size, std.testing.allocator));
}
