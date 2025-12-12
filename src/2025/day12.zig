const std = @import("std");
const utils = @import("utils");

pub fn main() !void {
    // https://adventofcode.com/2025/day/12
    utils.printTitle(2025, 12, "Christmas Tree Farm");

    const m = utils.Monitor.init();
    defer m.deinit();
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const ally = gpa.allocator();
    const input = comptime utils.trimInput(@embedFile("input"));

    const p1 = try part1(ally, input);
    utils.printAnswer(1, p1);
    // std.debug.assert(0 == p1);

    const p2 = try part2(ally, input);
    utils.printAnswer(2, p2);
    // std.debug.assert(0 == p2);
}

const Int = u64;

const Shape = struct {
    id: u8,
    grid: [3][3]bool,
    area: Int,
};

const Region = struct {
    width: usize,
    height: usize,
    quantities: []usize,

    fn deinit(self: *@This(), ally: std.mem.Allocator) void {
        ally.free(self.quantities);
    }
};

const Farm = struct {
    shapes: []Shape,
    regions: []Region,
    ally: std.mem.Allocator,

    fn parse(ally: std.mem.Allocator, input: []const u8) !Farm {
        var shapes = std.ArrayList(Shape).empty;
        var lineIt = std.mem.splitAny(u8, input, "\r\n");

        for (0..6) |_| {
            const firstLine = lineIt.next().?;
            const id = try std.fmt.parseInt(u8, firstLine[0 .. firstLine.len - 1], 10);
            var shape = Shape{ .id = id, .grid = undefined, .area = 0 };

            var y: usize = 0;
            while (lineIt.next()) |line| {
                if (line.len == 0) break;
                for (0.., line) |x, c| {
                    const on = c == '#';
                    shape.grid[y][x] = on;
                    if (on) shape.area += 1;
                }
                y += 1;
            }

            try shapes.append(ally, shape);
        }

        var quantityList = std.ArrayList(usize).empty;
        var regions = std.ArrayList(Region).empty;

        while (lineIt.next()) |line| {
            var it = std.mem.splitScalar(u8, line, ':');
            const size = it.next().?;
            const quantities = it.next().?;

            var it2 = std.mem.tokenizeScalar(u8, quantities, ' ');
            while (it2.next()) |q| {
                const qty = try std.fmt.parseInt(u8, q, 10);
                try quantityList.append(ally, qty);
            }

            var it3 = std.mem.splitScalar(u8, size, 'x');
            try regions.append(ally, Region{
                .width = try std.fmt.parseInt(usize, it3.next().?, 10),
                .height = try std.fmt.parseInt(usize, it3.next().?, 10),
                .quantities = try quantityList.toOwnedSlice(ally),
            });
        }

        return Farm{
            .shapes = try shapes.toOwnedSlice(ally),
            .regions = try regions.toOwnedSlice(ally),
            .ally = ally,
        };
    }

    fn deinit(self: *@This()) void {
        for (self.regions) |*r| {
            r.deinit(self.ally);
        }

        self.ally.free(self.shapes);
        self.ally.free(self.regions);
    }

    fn countRegions(self: *const @This()) Int {
        var total: Int = 0;

        // std.debug.print("debug\n", .{});

        for (self.regions) |r| {
            var minArea: Int = 0;

            for (0.., r.quantities) |i, q| {
                minArea += self.shapes[i].area * q;
            }

            const area = r.width * r.height;
            // std.debug.print("area = {}, minArea = {}\n", .{ area, minArea });

            if (area <= minArea) {
                total += 1;
            }
        }

        return total;
    }
};

fn part1(ally: std.mem.Allocator, input: []const u8) !Int {
    var farm = try Farm.parse(ally, input);
    defer farm.deinit();
    return farm.countRegions();
}

fn part2(ally: std.mem.Allocator, input: []const u8) !Int {
    _ = ally;

    var lineIt = std.mem.tokenizeAny(u8, input, "\r\n");
    while (lineIt.next()) |line| {
        _ = line;
    }

    return 0;
}

test "parts 1,2" {
    const ally = std.testing.allocator;
    const example =
        \\0:
        \\###
        \\##.
        \\##.
        \\
        \\1:
        \\###
        \\##.
        \\.##
        \\
        \\2:
        \\.##
        \\###
        \\##.
        \\
        \\3:
        \\##.
        \\###
        \\##.
        \\
        \\4:
        \\###
        \\#..
        \\###
        \\
        \\5:
        \\###
        \\.#.
        \\###
        \\
        \\4x4: 0 0 0 0 2 0
        \\12x5: 1 0 1 0 2 2
        \\12x5: 1 0 1 0 3 2
    ;
    try std.testing.expectEqual(2, try part1(ally, example));
    try std.testing.expectEqual(0, try part2(ally, example));
}
