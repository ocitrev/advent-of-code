const std = @import("std");

fn addAoc(b: *std.Build, year: u16, day: u8, target: anytype, optimize: anytype) void {
    const sourceFile = b.fmt("src/{}/day{}.zig", .{ year, day });
    const exe = b.addExecutable(.{
        .name = b.fmt("{}-{}", .{ year, day }),
        .root_source_file = .{ .path = sourceFile },
        .target = target,
        .optimize = optimize,
    });

    const inputFile = b.fmt("inputs/{}/day{}.txt", .{ year, day });
    exe.addAnonymousModule("input", .{ .source_file = std.build.FileSource.relative(inputFile) });
    b.installArtifact(exe);
    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }
    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);

    const unit_tests = b.addTest(.{
        .root_source_file = .{ .path = sourceFile },
        .target = target,
        .optimize = optimize,
    });
    const run_unit_tests = b.addRunArtifact(unit_tests);
    const test_step = b.step("test", "Run unit tests");
    test_step.dependOn(&run_unit_tests.step);
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

    addAoc(b, 2023, 1, target, optimize);
}
