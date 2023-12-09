const std = @import("std");

fn addAoc(b: *std.Build, year: u16, day: u8, target: anytype, optimize: anytype, run_step: anytype, test_step: anytype) void {
    const source_file = b.fmt("src/{}/day{}.zig", .{ year, day });
    const exe = b.addExecutable(.{
        .name = b.fmt("{}-{}", .{ year, day }),
        .root_source_file = .{ .path = source_file },
        .target = target,
        .optimize = optimize,
    });

    const input_file = b.fmt("inputs/{}/day{}.txt", .{ year, day });
    exe.addAnonymousModule("input", .{ .source_file = std.build.FileSource.relative(input_file) });
    b.installArtifact(exe);
    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }
    run_step.dependOn(&run_cmd.step);

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
        .root_source_file = .{ .path = source_file },
        .target = target,
        .optimize = optimize,
    });
    const runUnitTests = b.addRunArtifact(unit_tests);
    test_step.dependOn(&runUnitTests.step);

    const test_day_step = b.step(b.fmt("test-{}-{}", .{ year, day }), b.fmt("Run unit tests for year {}, day {}", .{ year, day }));
    test_day_step.dependOn(&runUnitTests.step);
}

// Although this function looks imperative, note that its job is to
// declaratively construct a build graph that will be executed by an external
// runner.
pub fn build(b: *std.Build) void {
    // Standard target options allows the person running `zig build` to choose
    // what target to build for. Here we do not override the defaults, which
    // means any target is allowed, and the default is native. Other options
    // for restricting supported target set are available.
    const target = b.standardTargetOptions(.{});

    // Standard optimization options allow the person running `zig build` to select
    // between Debug, ReleaseSafe, ReleaseFast, and ReleaseSmall. Here we do not
    // set a preferred release mode, allowing the user to decide how to optimize.
    const optimize = b.standardOptimizeOption(.{});

    const run_step = b.step("run", "Run the apps");
    const test_step = b.step("test", "Run unit tests");

    addAoc(b, 2023, 1, target, optimize, run_step, test_step);
    addAoc(b, 2023, 2, target, optimize, run_step, test_step);
    addAoc(b, 2023, 3, target, optimize, run_step, test_step);
    addAoc(b, 2023, 8, target, optimize, run_step, test_step);
}
