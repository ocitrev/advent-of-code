const std = @import("std");
const builtin = @import("builtin");
const platform_windows = @import("platform-windows.zig");
const platform_linux = @import("platform-linux.zig");

pub const ClipboardMode = enum {
    default,
    private,
};

pub fn setClipboardText(text: []const u8, mode: ClipboardMode) !void {
    if (comptime builtin.os.tag == .windows) {
        try platform_windows.setClipboardText(text, mode);
    } else {
        try platform_linux.setClipboardText(text, mode);
    }
}

pub const FileType = enum {
    stdin,
    stdout,
    stderr,
};

pub fn getOrEnableAnsiEscapeSupport(fileType: FileType) bool {
    if (comptime builtin.os.tag == .windows) {
        return platform_windows.getOrEnableAnsiEscapeSupport(fileType);
    } else {
        return true;
    }
}
