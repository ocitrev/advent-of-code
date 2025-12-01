const std = @import("std");

const BuildParams = struct {
    target: std.Build.ResolvedTarget,
    optimize: std.builtin.OptimizeMode,
};

const Aoc = struct {
    year: u16,
    day: u8,

    fn addTo(self: *const @This(), b: *std.Build, params: BuildParams, run_step: *std.Build.Step) void {
        const source_file = b.path(b.fmt("src/{}/day{}.zig", .{ self.year, self.day }));
        const exe = b.addExecutable(.{
            .name = b.fmt("{}-{}", .{ self.year, self.day }),
            .root_module = b.createModule(.{
                .root_source_file = source_file,
                .target = params.target,
                .optimize = params.optimize,
            }),
        });

        b.installArtifact(exe);

        if (b.modules.get("utils")) |utils| {
            exe.root_module.addImport("utils", utils);
        }

        // input file
        const input_file = b.path(b.fmt("inputs/{}/day{}.txt", .{ self.year, self.day }));
        exe.root_module.addAnonymousImport("input", .{ .root_source_file = input_file });

        // add to run step
        const run_cmd_batch = b.addRunArtifact(exe);
        run_cmd_batch.step.dependOn(b.getInstallStep());
        if (b.args) |args| {
            run_cmd_batch.addArgs(args);
        } else {
            const batch_args = [_][]const u8{"--batch"};
            run_cmd_batch.addArgs(&batch_args);
        }
        run_step.dependOn(&run_cmd_batch.step);

        // add or create year run step
        const run_year_step_name = b.fmt("run-{}", .{self.year});
        if (b.top_level_steps.get(run_year_step_name)) |step_info| {
            step_info.step.dependOn(&run_cmd_batch.step);
        } else {
            const run_year_step = b.step(run_year_step_name, b.fmt("Run apps for year {}", .{self.year}));
            run_year_step.dependOn(&run_cmd_batch.step);
        }

        // create day run step
        const run_cmd_standalone = b.addRunArtifact(exe);
        run_cmd_standalone.step.dependOn(b.getInstallStep());
        if (b.args) |args| {
            run_cmd_standalone.addArgs(args);
        }
        const run_day_step = b.step(b.fmt("run-{}-{}", .{ self.year, self.day }), b.fmt("Run app for year {}, day {}", .{ self.year, self.day }));
        run_day_step.dependOn(&run_cmd_standalone.step);
    }

    fn addTestsTo(self: *const @This(), b: *std.Build, params: BuildParams, test_step: *std.Build.Step) void {
        const source_file = b.path(b.fmt("src/{}/day{}.zig", .{ self.year, self.day }));

        // unit tests
        const unit_tests = b.addTest(.{
            .root_module = b.createModule(.{
                .root_source_file = source_file,
                .target = params.target,
                .optimize = params.optimize,
            }),
        });

        // make sure utils module is available in unit tests
        if (b.modules.get("utils")) |utils| {
            unit_tests.root_module.addImport("utils", utils);
        }

        const run_unit_tests = b.addRunArtifact(unit_tests);
        test_step.dependOn(&run_unit_tests.step);

        // add or create year test step
        const test_year_step_name = b.fmt("test-{}", .{self.year});
        if (b.top_level_steps.get(test_year_step_name)) |step_info| {
            step_info.step.dependOn(&run_unit_tests.step);
        } else {
            const test_year_step = b.step(test_year_step_name, b.fmt("Run unit tests for year {}", .{self.year}));
            test_year_step.dependOn(&run_unit_tests.step);
        }

        // create day test step
        const test_day_step = b.step(b.fmt("test-{}-{}", .{ self.year, self.day }), b.fmt("Run unit tests for year {}, day {}", .{ self.year, self.day }));
        test_day_step.dependOn(&run_unit_tests.step);
    }
};

pub fn build(b: *std.Build) void {
    const puzzles = [_]Aoc{
        .{ .year = 2016, .day = 5 },
        .{ .year = 2016, .day = 8 },

        .{ .year = 2017, .day = 1 },
        .{ .year = 2017, .day = 2 },

        .{ .year = 2019, .day = 1 },

        .{ .year = 2023, .day = 1 },
        .{ .year = 2023, .day = 2 },
        .{ .year = 2023, .day = 3 },
        .{ .year = 2023, .day = 4 },
        .{ .year = 2023, .day = 8 },
        .{ .year = 2023, .day = 9 },
        .{ .year = 2023, .day = 10 },
        .{ .year = 2023, .day = 11 },

        .{ .year = 2024, .day = 1 },
        .{ .year = 2024, .day = 2 },
        .{ .year = 2024, .day = 3 },
        .{ .year = 2024, .day = 4 },
        .{ .year = 2024, .day = 5 },
        .{ .year = 2024, .day = 6 },
        .{ .year = 2024, .day = 7 },
        .{ .year = 2024, .day = 8 },
        .{ .year = 2024, .day = 9 },
        .{ .year = 2024, .day = 10 },
        .{ .year = 2024, .day = 11 },
        .{ .year = 2024, .day = 12 },
        .{ .year = 2024, .day = 13 },
        .{ .year = 2024, .day = 14 },
        .{ .year = 2024, .day = 15 },
        .{ .year = 2024, .day = 16 },
        .{ .year = 2024, .day = 17 },
        .{ .year = 2024, .day = 18 },
        .{ .year = 2024, .day = 19 },
        .{ .year = 2024, .day = 20 },
        .{ .year = 2024, .day = 21 },
        .{ .year = 2024, .day = 22 },
        .{ .year = 2024, .day = 23 },
        .{ .year = 2024, .day = 24 },
        .{ .year = 2024, .day = 25 },

        .{ .year = 2025, .day = 1 },
    };

    const params = BuildParams{
        .target = b.standardTargetOptions(.{}),
        .optimize = b.standardOptimizeOption(.{}),
    };

    const utils = b.addModule("utils", .{
        .root_source_file = b.path("src/utils.zig"),
    });

    const run_step = b.step("run", "Run all apps");

    const test_step = b.step("test", "Run all unit tests");
    const utils_tests = b.addTest(.{
        .root_module = b.createModule(.{
            .root_source_file = utils.root_source_file.?,
            .target = params.target,
            .optimize = params.optimize,
        }),
    });
    test_step.dependOn(&b.addRunArtifact(utils_tests).step);

    for (puzzles) |p| {
        p.addTo(b, params, run_step);
        p.addTestsTo(b, params, test_step);
    }
}
