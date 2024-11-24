const std = @import("std");

const BuildParams = struct {
    target: std.Build.ResolvedTarget,
    optimize: std.builtin.OptimizeMode,
};

fn addAoc(b: *std.Build, year: u16, day: u8, params: BuildParams, run_step: *std.Build.Step) void {
    const source_file = b.path(b.fmt("src/{}/day{}.zig", .{ year, day }));
    const exe = b.addExecutable(.{
        .name = b.fmt("{}-{}", .{ year, day }),
        .root_source_file = source_file,
        .target = params.target,
        .optimize = params.optimize,
    });

    b.installArtifact(exe);

    if (b.modules.get("utils")) |utils| {
        exe.root_module.addImport("utils", utils);
    }

    // input file
    const input_file = b.path(b.fmt("inputs/{}/day{}.txt", .{ year, day }));
    exe.root_module.addAnonymousImport("input", .{ .root_source_file = input_file });

    // add to run step
    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }
    run_step.dependOn(&run_cmd.step);

    // add or create year run step
    const run_year_step_name = b.fmt("run-{}", .{year});
    if (b.top_level_steps.get(run_year_step_name)) |step_info| {
        step_info.step.dependOn(&run_cmd.step);
    } else {
        const run_year_step = b.step(run_year_step_name, b.fmt("Run apps for year {}", .{year}));
        run_year_step.dependOn(&run_cmd.step);
    }

    // create day run step
    const run_day_step = b.step(b.fmt("run-{}-{}", .{ year, day }), b.fmt("Run app for year {}, day {}", .{ year, day }));
    run_day_step.dependOn(&run_cmd.step);
}

fn addAocTests(b: *std.Build, year: u16, day: u8, params: BuildParams, test_step: *std.Build.Step) void {
    const source_file = b.path(b.fmt("src/{}/day{}.zig", .{ year, day }));

    // unit tests
    const unit_tests = b.addTest(.{
        .root_source_file = source_file,
        .target = params.target,
        .optimize = params.optimize,
    });

    // make sure utils module is available in unit tests
    if (b.modules.get("utils")) |utils| {
        unit_tests.root_module.addImport("utils", utils);
    }

    const run_unit_tests = b.addRunArtifact(unit_tests);
    test_step.dependOn(&run_unit_tests.step);

    // add or create year test step
    const test_year_step_name = b.fmt("test-{}", .{year});
    if (b.top_level_steps.get(test_year_step_name)) |step_info| {
        step_info.step.dependOn(&run_unit_tests.step);
    } else {
        const test_year_step = b.step(test_year_step_name, b.fmt("Run unit tests for year {}", .{year}));
        test_year_step.dependOn(&run_unit_tests.step);
    }

    // create day test step
    const test_day_step = b.step(b.fmt("test-{}-{}", .{ year, day }), b.fmt("Run unit tests for year {}, day {}", .{ year, day }));
    test_day_step.dependOn(&run_unit_tests.step);
}

// Although this function looks imperative, note that its job is to
// declaratively construct a build graph that will be executed by an external
// runner.
pub fn build(b: *std.Build) void {
    const params = BuildParams{
        .target = b.standardTargetOptions(.{}),
        .optimize = b.standardOptimizeOption(.{}),
    };

    const utils = b.addModule("utils", .{
        .root_source_file = b.path("src/utils.zig"),
    });

    const run_step = b.step("run", "Run all apps");
    addAoc(b, 2016, 5, params, run_step);
    addAoc(b, 2016, 8, params, run_step);
    addAoc(b, 2023, 1, params, run_step);
    addAoc(b, 2023, 2, params, run_step);
    addAoc(b, 2023, 3, params, run_step);
    addAoc(b, 2023, 4, params, run_step);
    addAoc(b, 2023, 8, params, run_step);
    addAoc(b, 2023, 9, params, run_step);
    addAoc(b, 2023, 10, params, run_step);
    addAoc(b, 2023, 11, params, run_step);

    const test_step = b.step("test", "Run all unit tests");
    const utils_tests = b.addTest(.{
        .root_source_file = utils.root_source_file.?,
        .optimize = params.optimize,
    });
    test_step.dependOn(&b.addRunArtifact(utils_tests).step);

    addAocTests(b, 2016, 5, params, test_step);
    addAocTests(b, 2016, 8, params, test_step);
    addAocTests(b, 2023, 1, params, test_step);
    addAocTests(b, 2023, 2, params, test_step);
    addAocTests(b, 2023, 3, params, test_step);
    addAocTests(b, 2023, 4, params, test_step);
    addAocTests(b, 2023, 8, params, test_step);
    addAocTests(b, 2023, 9, params, test_step);
    addAocTests(b, 2023, 10, params, test_step);
    addAocTests(b, 2023, 11, params, test_step);
}
