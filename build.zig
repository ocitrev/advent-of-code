const std = @import("std");

const ModuleDep = struct {
    name: []const u8,
    mod: *std.Build.Module,
};

const BuildParams = struct {
    target: std.zig.CrossTarget,
    optimize: std.builtin.OptimizeMode,
    run_step: *std.Build.Step,
    test_step: *std.Build.Step,
    module: ModuleDep,
};

fn addAoc(b: *std.Build, year: u16, day: u8, params: BuildParams) void {
    const source_file = std.Build.LazyPath{ .path = b.fmt("src/{}/day{}.zig", .{ year, day }) };
    const exe = b.addExecutable(.{
        .name = b.fmt("{}-{}", .{ year, day }),
        .root_source_file = source_file,
        .target = params.target,
        .optimize = params.optimize,
    });

    exe.addModule(params.module.name, params.module.mod);

    const input_file = b.fmt("inputs/{}/day{}.txt", .{ year, day });
    exe.addAnonymousModule("input", .{ .source_file = std.Build.FileSource.relative(input_file) });
    b.installArtifact(exe);
    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }
    params.run_step.dependOn(&run_cmd.step);

    const run_year_step_name = b.fmt("run-{}", .{year});
    if (b.top_level_steps.get(run_year_step_name)) |step_info| {
        step_info.step.dependOn(&run_cmd.step);
    } else {
        const run_year_step = b.step(run_year_step_name, b.fmt("Run year {}", .{year}));
        run_year_step.dependOn(&run_cmd.step);
    }

    const run_day_step = b.step(b.fmt("run-{}-{}", .{ year, day }), b.fmt("Run year {}, day {}", .{ year, day }));
    run_day_step.dependOn(&run_cmd.step);

    const unit_tests = b.addTest(.{
        .root_source_file = source_file,
        .target = params.target,
        .optimize = params.optimize,
    });

    unit_tests.addModule(params.module.name, params.module.mod);

    const runUnitTests = b.addRunArtifact(unit_tests);
    params.test_step.dependOn(&runUnitTests.step);

    const test_day_step = b.step(b.fmt("test-{}-{}", .{ year, day }), b.fmt("Run unit tests for year {}, day {}", .{ year, day }));
    test_day_step.dependOn(&runUnitTests.step);
}

// Although this function looks imperative, note that its job is to
// declaratively construct a build graph that will be executed by an external
// runner.
pub fn build(b: *std.Build) void {
    const utils = b.createModule(.{
        .source_file = std.Build.FileSource.relative("src/utils.zig"),
    });

    const params = BuildParams{
        .target = b.standardTargetOptions(.{}),
        .optimize = b.standardOptimizeOption(.{}),
        .run_step = b.step("run", "Run the apps"),
        .test_step = b.step("test", "Run unit tests"),
        .module = ModuleDep{ .name = "utils", .mod = utils },
    };

    addAoc(b, 2023, 1, params);
    addAoc(b, 2023, 2, params);
    addAoc(b, 2023, 3, params);
    addAoc(b, 2023, 4, params);
    addAoc(b, 2023, 8, params);
}
