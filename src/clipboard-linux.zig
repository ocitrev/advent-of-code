const std = @import("std");
const builtin = @import("builtin");
const clipboard = @import("clipboard.zig");

const diagnostics = switch (builtin.os.tag) {
    .linux => {},
    else => @compileError("Not supported on this platform"),
};

pub fn setClipboardText(text: []const u8, mode: clipboard.ClipboardMode) !void {
    _ = text;
    _ = mode;
}
