const std = @import("std");
const builtin = @import("builtin");
const platform = @import("platform.zig");

const diagnostics = switch (builtin.os.tag) {
    .linux => {},
    else => @compileError("Not supported on this platform"),
};

pub fn setClipboardText(text: []const u8, mode: platform.ClipboardMode) !void {
    _ = text;
    _ = mode;
}
