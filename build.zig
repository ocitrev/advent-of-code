const std = @import("std");

const BuildParams = struct {
    target: std.Build.ResolvedTarget,
    optimize: std.builtin.OptimizeMode,
};

const Type = enum {
    cpp,
    zig,
};

var cppUtilsLib: ?*std.Build.Step.Compile = null;

const CppDependency = struct {
    name: []const u8,
    includePath: []const u8,
};

const Aoc = struct {
    year: u16,
    day: u8,
    type: Type = .zig,
    deps: []const CppDependency = &.{},

    fn addRunSteps(self: *const @This(), b: *std.Build, runStep: *std.Build.Step, exe: *std.Build.Step.Compile) void {
        // add to run step
        const run_cmd_batch = b.addRunArtifact(exe);
        run_cmd_batch.step.dependOn(b.getInstallStep());
        if (b.args) |args| {
            run_cmd_batch.addArgs(args);
        } else {
            const batch_args = [_][]const u8{"--batch"};
            run_cmd_batch.addArgs(&batch_args);
        }
        runStep.dependOn(&run_cmd_batch.step);

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

    fn addZigTo(self: *const @This(), b: *std.Build, params: BuildParams, runStep: *std.Build.Step) void {
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

        self.addRunSteps(b, runStep, exe);
    }

    fn addZigTestsTo(self: *const @This(), b: *std.Build, params: BuildParams, testStep: *std.Build.Step) void {
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
        testStep.dependOn(&run_unit_tests.step);

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

    fn generateInputHeader(self: *const @This(), b: *std.Build) std.Build.LazyPath {
        const fileWriter = b.addWriteFiles();
        const inputFile = b.fmt("inputs/{}/day{}.txt", .{ self.year, self.day });

        const fullPath = b.path(inputFile).getPath2(b, null);
        const bytes = std.fs.cwd().readFileAlloc(b.allocator, fullPath, 1024 * 1024) catch {
            std.debug.panic("Failed to read input file: {any}", .{fullPath});
        };

        const text = b.fmt(
            \\#pragma once
            \\
            \\// auto-generated file
            \\#include <string_view>
            \\
            \\static constexpr char inputBytes[]{any};
            \\
            \\constexpr std::string_view GetInput()
            \\{{
            \\    std::string_view text{{inputBytes, sizeof(inputBytes)}};
            \\
            \\    if (auto const pos = text.find_last_not_of("\r\n"); pos != text.npos)
            \\    {{
            \\        text = text.substr(0, pos + 1);
            \\    }}
            \\
            \\    return text;
            \\}}
        , .{bytes});

        return fileWriter.add("input-bytes.hpp", text);
    }

    const cppFlags = &[_][]const u8{
        "-std=c++23",
        "-Wall",
        "-DNOMINMAX",
        "-DWIN32_LEAN_AND_MEAN",
    };

    fn addCppUtils(b: *std.Build, params: BuildParams) *std.Build.Step.Compile {
        if (cppUtilsLib) |lib| {
            return lib;
        }

        const cppUtils = b.addLibrary(.{
            .name = "cppUtils",
            .root_module = b.createModule(.{
                .target = params.target,
                .optimize = params.optimize,
            }),
            .linkage = .static,
        });

        cppUtils.addCSourceFiles(.{
            .files = &.{
                "src/common/intcode.cpp",
                "src/common/string.cpp",
                "src/common/terminal.cpp",
            },
            .flags = cppFlags,
            .language = .cpp,
        });
        cppUtils.addCSourceFiles(.{
            .files = &.{
                "src/common/md5.c",
            },
            .flags = &.{"-Wall"},
            .language = .c,
        });

        cppUtils.linkLibC();
        cppUtils.linkLibCpp();
        cppUtilsLib = cppUtils;
        return cppUtils;
    }

    fn addCppTo(self: *const @This(), b: *std.Build, params: BuildParams, runStep: *std.Build.Step) void {
        const exe = b.addExecutable(.{
            .name = b.fmt("{}-{}", .{ self.year, self.day }),
            .root_module = b.createModule(.{
                .target = params.target,
                .optimize = params.optimize,
            }),
        });

        const generatedHeader = self.generateInputHeader(b);
        exe.addIncludePath(generatedHeader.dirname());

        const sourceFile = b.fmt("src/{}/day{}.cpp", .{ self.year, self.day });
        exe.addCSourceFile(.{
            .file = .{
                .src_path = .{
                    .owner = b,
                    .sub_path = sourceFile,
                },
            },
            .flags = cppFlags,
            .language = .cpp,
        });

        for (self.deps) |dep| {
            const lazyDep = b.lazyDependency(dep.name, .{
                .target = params.target,
                .optimize = params.optimize,
            });
            if (lazyDep) |resolved| {
                exe.addSystemIncludePath(resolved.path(dep.includePath));
            }
        }

        exe.linkLibrary(addCppUtils(b, params));
        exe.linkLibC();
        exe.linkLibCpp();

        b.installArtifact(exe);

        self.addRunSteps(b, runStep, exe);
    }
};

const Lang = enum {
    all,
    cpp,
    zig,
};

pub fn build(b: *std.Build) void {

    // Add custom options
    const langOption = b.option(
        Lang,
        "lang",
        "Language to build (zig, cpp, or all)",
    ) orelse .zig;

    const puzzles = [_]Aoc{
        .{ .year = 2015, .day = 1, .type = .cpp },
        .{ .year = 2015, .day = 2, .type = .cpp },
        .{ .year = 2015, .day = 3, .type = .cpp },
        .{ .year = 2015, .day = 4, .type = .cpp },
        .{ .year = 2015, .day = 5, .type = .cpp },
        .{ .year = 2015, .day = 6, .type = .cpp },
        .{ .year = 2015, .day = 7, .type = .cpp },
        .{ .year = 2015, .day = 8, .type = .cpp },
        .{ .year = 2015, .day = 9, .type = .cpp },
        .{ .year = 2015, .day = 10, .type = .cpp },
        .{ .year = 2015, .day = 11, .type = .cpp },
        .{ .year = 2015, .day = 12, .type = .cpp, .deps = &[_]CppDependency{
            .{
                .name = "json",
                .includePath = "include",
            },
        } },
        .{ .year = 2015, .day = 13, .type = .cpp },
        .{ .year = 2015, .day = 14, .type = .cpp },
        .{ .year = 2015, .day = 15, .type = .cpp },
        .{ .year = 2015, .day = 16, .type = .cpp },
        .{ .year = 2015, .day = 17, .type = .cpp },
        .{ .year = 2015, .day = 18, .type = .cpp },
        .{ .year = 2015, .day = 19, .type = .cpp },
        .{ .year = 2015, .day = 20, .type = .cpp },
        .{ .year = 2015, .day = 21, .type = .cpp },
        .{ .year = 2015, .day = 22, .type = .cpp },
        .{ .year = 2015, .day = 23, .type = .cpp },
        .{ .year = 2015, .day = 24, .type = .cpp },
        .{ .year = 2015, .day = 25, .type = .cpp },

        .{ .year = 2016, .day = 1, .type = .cpp },
        .{ .year = 2016, .day = 2, .type = .cpp },
        .{ .year = 2016, .day = 3, .type = .cpp },
        .{ .year = 2016, .day = 4, .type = .cpp },
        .{ .year = 2016, .day = 5 },
        .{ .year = 2016, .day = 8 },

        .{ .year = 2017, .day = 1 },
        .{ .year = 2017, .day = 2 },

        .{ .year = 2019, .day = 1, .type = .cpp },
        .{ .year = 2019, .day = 2, .type = .cpp },
        .{ .year = 2019, .day = 3, .type = .cpp },
        .{ .year = 2019, .day = 4, .type = .cpp },
        .{ .year = 2019, .day = 5, .type = .cpp },
        .{ .year = 2019, .day = 6, .type = .cpp },
        .{ .year = 2019, .day = 7, .type = .cpp },
        .{ .year = 2019, .day = 8, .type = .cpp },
        .{ .year = 2019, .day = 9, .type = .cpp },
        .{ .year = 2019, .day = 10, .type = .cpp },
        .{ .year = 2019, .day = 11, .type = .cpp },
        .{ .year = 2019, .day = 12, .type = .cpp },
        .{ .year = 2019, .day = 13, .type = .cpp },
        .{ .year = 2019, .day = 14, .type = .cpp },
        .{ .year = 2019, .day = 15, .type = .cpp },
        .{ .year = 2019, .day = 16, .type = .cpp },
        .{ .year = 2019, .day = 17, .type = .cpp },
        .{ .year = 2019, .day = 19, .type = .cpp },

        .{ .year = 2020, .day = 1, .type = .cpp },
        .{ .year = 2020, .day = 2, .type = .cpp },
        .{ .year = 2020, .day = 3, .type = .cpp },
        .{ .year = 2020, .day = 4, .type = .cpp },
        .{ .year = 2020, .day = 5, .type = .cpp },
        .{ .year = 2020, .day = 6, .type = .cpp },
        .{ .year = 2020, .day = 7, .type = .cpp },
        .{ .year = 2020, .day = 8, .type = .cpp },
        .{ .year = 2020, .day = 9, .type = .cpp },
        .{ .year = 2020, .day = 10, .type = .cpp },
        .{ .year = 2020, .day = 11, .type = .cpp },
        .{ .year = 2020, .day = 12, .type = .cpp },
        .{ .year = 2020, .day = 13, .type = .cpp },
        .{ .year = 2020, .day = 14, .type = .cpp },
        .{ .year = 2020, .day = 15, .type = .cpp },
        .{ .year = 2020, .day = 16, .type = .cpp },
        .{ .year = 2020, .day = 17, .type = .cpp },
        .{ .year = 2020, .day = 18, .type = .cpp },
        .{ .year = 2020, .day = 19, .type = .cpp },

        .{ .year = 2021, .day = 1, .type = .cpp },
        .{ .year = 2021, .day = 2, .type = .cpp },
        .{ .year = 2021, .day = 3, .type = .cpp },
        .{ .year = 2021, .day = 4, .type = .cpp },
        .{ .year = 2021, .day = 5, .type = .cpp },
        .{ .year = 2021, .day = 6, .type = .cpp },
        .{ .year = 2021, .day = 7, .type = .cpp },
        .{ .year = 2021, .day = 8, .type = .cpp },
        .{ .year = 2021, .day = 9, .type = .cpp },
        .{ .year = 2021, .day = 10, .type = .cpp },
        .{ .year = 2021, .day = 11, .type = .cpp },
        .{ .year = 2021, .day = 12, .type = .cpp },
        .{ .year = 2021, .day = 13, .type = .cpp },
        .{ .year = 2021, .day = 14, .type = .cpp },
        .{ .year = 2021, .day = 15, .type = .cpp },

        .{ .year = 2022, .day = 1, .type = .cpp },
        .{ .year = 2022, .day = 2, .type = .cpp },
        .{ .year = 2022, .day = 3, .type = .cpp },
        .{ .year = 2022, .day = 4, .type = .cpp },
        .{ .year = 2022, .day = 5, .type = .cpp },
        .{ .year = 2022, .day = 6, .type = .cpp },
        .{ .year = 2022, .day = 7, .type = .cpp },
        .{ .year = 2022, .day = 8, .type = .cpp },
        .{ .year = 2022, .day = 9, .type = .cpp },
        .{ .year = 2022, .day = 10, .type = .cpp },
        .{ .year = 2022, .day = 11, .type = .cpp },
        .{ .year = 2022, .day = 12, .type = .cpp },
        .{ .year = 2022, .day = 13, .type = .cpp },
        .{ .year = 2022, .day = 14, .type = .cpp },
        .{ .year = 2022, .day = 15, .type = .cpp, .deps = &[_]CppDependency{
            .{
                .name = "ctre",
                .includePath = "single-header",
            },
        } },
        .{ .year = 2022, .day = 16, .type = .cpp },

        .{ .year = 2023, .day = 1 },
        // .{ .year = 2023, .day = 1, .type = .cpp },
        .{ .year = 2023, .day = 2 },
        // .{ .year = 2023, .day = 2, .type = .cpp },
        .{ .year = 2023, .day = 3 },
        // .{ .year = 2023, .day = 3, .type = .cpp },
        .{ .year = 2023, .day = 4 },
        // .{ .year = 2023, .day = 4, .type = .cpp },
        .{ .year = 2023, .day = 5, .type = .cpp },
        .{ .year = 2023, .day = 6, .type = .cpp },
        .{ .year = 2023, .day = 7, .type = .cpp },
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
        .{ .year = 2025, .day = 2 },
        .{ .year = 2025, .day = 3 },
        .{ .year = 2025, .day = 4 },
        .{ .year = 2025, .day = 5 },
        .{ .year = 2025, .day = 6 },
        .{ .year = 2025, .day = 7 },
    };

    const params = BuildParams{
        .target = b.standardTargetOptions(.{}),
        .optimize = b.standardOptimizeOption(.{}),
    };

    const utils = b.addModule("utils", .{
        .root_source_file = b.path("src/utils.zig"),
    });

    const runStep = b.step("run", "Run all apps");

    const testStep = b.step("test", "Run all unit tests");
    const utils_tests = b.addTest(.{
        .root_module = b.createModule(.{
            .root_source_file = utils.root_source_file.?,
            .target = params.target,
            .optimize = params.optimize,
        }),
    });
    testStep.dependOn(&b.addRunArtifact(utils_tests).step);

    for (puzzles) |p| {
        switch (p.type) {
            .cpp => {
                switch (langOption) {
                    .zig => continue,
                    else => p.addCppTo(b, params, runStep),
                }
            },
            .zig => {
                switch (langOption) {
                    .cpp => continue,
                    else => {
                        p.addZigTo(b, params, runStep);
                        p.addZigTestsTo(b, params, testStep);
                    },
                }
            },
        }
    }
}
