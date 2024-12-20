const std = @import("std");
const builtin = @import("builtin");
const windows = std.os.windows;

pub const ClipboardMode = enum {
    default,
    private,
};

const diagnostics = switch (builtin.os.tag) {
    .windows => {},
    else => @compileError("Not supported on this platform"),
};

extern "user32" fn OpenClipboard(hwnd: ?windows.HWND) callconv(windows.WINAPI) windows.BOOL;
extern "user32" fn CloseClipboard() callconv(windows.WINAPI) windows.BOOL;
extern "user32" fn EmptyClipboard() callconv(windows.WINAPI) windows.BOOL;
extern "user32" fn RegisterClipboardFormatW(windows.LPCWSTR) callconv(windows.WINAPI) windows.UINT;
extern "user32" fn SetClipboardData(format: windows.UINT, handle: windows.HANDLE) callconv(windows.WINAPI) ?windows.HANDLE;
extern "kernel32" fn GlobalLock(handle: windows.HANDLE) callconv(windows.WINAPI) ?*anyopaque;
extern "kernel32" fn GlobalUnlock(handle: windows.HANDLE) callconv(windows.WINAPI) windows.BOOL;
extern "kernel32" fn GlobalAlloc(flags: windows.UINT, size: windows.SIZE_T) callconv(windows.WINAPI) ?*anyopaque;
extern "kernel32" fn RtlMoveMemory(out: *anyopaque, in: *anyopaque, size: windows.SIZE_T) callconv(windows.WINAPI) void;

fn registerClipboardFormat(format: []const u8) !windows.UINT {
    const formatW = try std.unicode.utf8ToUtf16LeAllocZ(std.heap.page_allocator, format);
    defer std.heap.page_allocator.free(formatW);
    return RegisterClipboardFormatW(formatW);
}

fn setClipboardData(format: windows.UINT, data: []u8) !void {
    const size: windows.SIZE_T = @intCast(data.len);
    const handle = GlobalAlloc(GMEM_MOVEABLE, size) orelse return error.GlobalAlloc;
    const locked = GlobalLock(handle) orelse return error.GlobalLock;
    RtlMoveMemory(locked, data.ptr, size);
    _ = GlobalUnlock(handle);
    _ = SetClipboardData(format, handle) orelse return error.SetClipboardData;
}

fn setClipboardDWORD(format: windows.UINT, data: windows.DWORD) !void {
    var bytes = std.mem.toBytes(data);
    try setClipboardData(format, &bytes);
}

const GMEM_MOVEABLE: windows.UINT = 0x0002;
const CF_UNICODE_TEXT: windows.UINT = 13;

pub fn setClipboardText(text: []const u8, mode: ClipboardMode) !void {
    if (text.len == 0) return;
    if (OpenClipboard(null) == 0) return error.OpenClipboard;
    defer _ = CloseClipboard();
    if (EmptyClipboard() == 0) return error.EmptyClipboard;

    if (mode == .private) {
        try setClipboardDWORD(try registerClipboardFormat("ExcludeClipboardContentFromMonitorProcessing"), 0);
        try setClipboardDWORD(try registerClipboardFormat("CanIncludeInClipboardHistory"), 0);
        try setClipboardDWORD(try registerClipboardFormat("CanUploadToCloudClipboard"), 0);
    }

    const textW = try std.unicode.utf8ToUtf16LeAllocZ(std.heap.page_allocator, text);
    const nbBytes = @sizeOf(u16) * (textW.len + 1);
    const handle = GlobalAlloc(GMEM_MOVEABLE, nbBytes) orelse return error.GlobalAlloc;
    const locked = GlobalLock(handle) orelse return error.GlobalLock;
    const aligned: [*:0]u16 = @alignCast(@ptrCast(locked));
    const span = std.mem.span(aligned);
    RtlMoveMemory(span.ptr, textW.ptr, nbBytes);
    _ = GlobalUnlock(handle);
    _ = SetClipboardData(CF_UNICODE_TEXT, handle) orelse return error.SetClipboardData;
}
