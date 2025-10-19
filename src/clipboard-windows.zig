const std = @import("std");
const builtin = @import("builtin");
const clipboard = @import("clipboard.zig");

const diagnostics = switch (builtin.os.tag) {
    .windows => {},
    else => @compileError("Not supported on this platform"),
};

const windows = std.os.windows;

extern "user32" fn OpenClipboard(hwnd: ?windows.HWND) callconv(.winapi) windows.BOOL;
extern "user32" fn CloseClipboard() callconv(.winapi) windows.BOOL;
extern "user32" fn EmptyClipboard() callconv(.winapi) windows.BOOL;
extern "user32" fn RegisterClipboardFormatW(windows.LPCWSTR) callconv(.winapi) windows.UINT;
extern "user32" fn SetClipboardData(format: windows.UINT, handle: windows.HANDLE) callconv(.winapi) ?windows.HANDLE;
extern "kernel32" fn GlobalLock(handle: windows.HANDLE) callconv(.winapi) ?*anyopaque;
extern "kernel32" fn GlobalUnlock(handle: windows.HANDLE) callconv(.winapi) windows.BOOL;
extern "kernel32" fn GlobalAlloc(flags: windows.UINT, size: windows.SIZE_T) callconv(.winapi) ?*anyopaque;
extern "kernel32" fn RtlMoveMemory(out: *anyopaque, in: *anyopaque, size: windows.SIZE_T) callconv(.winapi) void;

fn registerClipboardFormat(format: []const u8) !windows.UINT {
    var buffer: [1024]u8 = undefined;
    var fba = std.heap.FixedBufferAllocator.init(&buffer);
    var gpa = fba.allocator();

    const formatW = try std.unicode.utf8ToUtf16LeAllocZ(gpa, format);
    defer gpa.free(formatW);
    return RegisterClipboardFormatW(formatW);
}

fn setClipboardData(format: windows.UINT, data: []u8) !void {
    const size: windows.SIZE_T = @intCast(data.len);
    const handle = GlobalAlloc(GMEM_MOVEABLE, size) orelse return error.GlobalAlloc;
    const locked = GlobalLock(handle) orelse return error.GlobalLock;
    defer _ = GlobalUnlock(handle);
    RtlMoveMemory(locked, data.ptr, size);
    _ = SetClipboardData(format, handle) orelse return error.SetClipboardData;
}

fn setClipboardDWORD(format: windows.UINT, data: windows.DWORD) !void {
    var bytes = std.mem.toBytes(data);
    try setClipboardData(format, &bytes);
}

const GMEM_MOVEABLE: windows.UINT = 0x0002;
const CF_UNICODE_TEXT: windows.UINT = 13;

pub fn setClipboardText(text: []const u8, mode: clipboard.ClipboardMode) !void {
    if (text.len == 0) return;
    if (OpenClipboard(null) == 0) return error.OpenClipboard;
    defer _ = CloseClipboard();
    if (EmptyClipboard() == 0) return error.EmptyClipboard;

    if (mode == .private) {
        try setClipboardDWORD(try registerClipboardFormat("ExcludeClipboardContentFromMonitorProcessing"), 0);
        try setClipboardDWORD(try registerClipboardFormat("CanIncludeInClipboardHistory"), 0);
        try setClipboardDWORD(try registerClipboardFormat("CanUploadToCloudClipboard"), 0);
    }

    var buffer: [1024]u8 = undefined;
    var fba = std.heap.FixedBufferAllocator.init(&buffer);
    var gpa = fba.allocator();

    const textW = try std.unicode.utf8ToUtf16LeAllocZ(gpa, text);
    defer gpa.free(textW);

    const u8ptr: [*]u8 = @ptrCast(textW.ptr);
    std.debug.print("got: {any}\n", .{u8ptr});

    const nbBytes = @sizeOf(u16) * (textW.len + 1);
    const handle = GlobalAlloc(GMEM_MOVEABLE, nbBytes) orelse return error.GlobalAlloc;
    const locked = GlobalLock(handle) orelse return error.GlobalLock;
    defer _ = GlobalUnlock(handle);
    RtlMoveMemory(locked, textW.ptr, nbBytes);
    _ = SetClipboardData(CF_UNICODE_TEXT, handle) orelse return error.SetClipboardData;
}
