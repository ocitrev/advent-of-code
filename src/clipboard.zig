const std = @import("std");
const builtin = @import("builtin");
const clipboard_windows = @import("clipboard-windows.zig");
const clipboard_linux = @import("clipboard-linux.zig");

pub const ClipboardMode = enum {
    default,
    private,
};

pub fn setClipboardText(text: []const u8, mode: ClipboardMode) !void {
    if (comptime builtin.os.tag == .windows) {
        try clipboard_windows.setClipboardText(text, mode);
    } else {
        try clipboard_linux.setClipboardText(text, mode);
    }
}
