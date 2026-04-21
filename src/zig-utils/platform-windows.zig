const std = @import("std");
const builtin = @import("builtin");
const platform = @import("platform.zig");

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
extern "kernel32" fn GetLastError() callconv(.winapi) windows.DWORD;
extern "kernel32" fn GlobalLock(handle: windows.HANDLE) callconv(.winapi) ?*anyopaque;
extern "kernel32" fn GlobalUnlock(handle: windows.HANDLE) callconv(.winapi) windows.BOOL;
extern "kernel32" fn GlobalAlloc(flags: windows.UINT, size: windows.SIZE_T) callconv(.winapi) ?*anyopaque;
extern "kernel32" fn RtlMoveMemory(out: *anyopaque, in: *anyopaque, size: windows.SIZE_T) callconv(.winapi) void;
extern "kernel32" fn SwitchToThread() callconv(.winapi) windows.BOOL;
extern "kernel32" fn GetStdHandle(nStdHandle: windows.DWORD) callconv(.winapi) ?windows.HANDLE;
extern "kernel32" fn GetConsoleMode(hConsoleHandle: windows.HANDLE, lpMode: *windows.DWORD) callconv(.winapi) windows.BOOL;
extern "kernel32" fn SetConsoleMode(hConsoleHandle: windows.HANDLE, dwMode: windows.DWORD) callconv(.winapi) windows.BOOL;

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

fn openClipboardWithErrorHandling() !void {
    if (OpenClipboard(null) != .FALSE) {
        return;
    }

    // Retry when access is denied, as another application might be holding the clipboard
    if (GetLastError() == 5) {
        for (0..3) |_| {
            _ = SwitchToThread();
            if (OpenClipboard(null) != .FALSE) return;
        }
    }

    return error.OpenClipboard;
}

pub fn setClipboardText(text: []const u8, mode: platform.ClipboardMode) !void {
    if (text.len == 0) return;
    try openClipboardWithErrorHandling();
    defer _ = CloseClipboard();
    if (EmptyClipboard() == .FALSE) return error.EmptyClipboard;

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

    const nbBytes = @sizeOf(u16) * (textW.len + 1);
    const handle = GlobalAlloc(GMEM_MOVEABLE, nbBytes) orelse return error.GlobalAlloc;
    const locked = GlobalLock(handle) orelse return error.GlobalLock;
    defer _ = GlobalUnlock(handle);
    RtlMoveMemory(locked, textW.ptr, nbBytes);
    _ = SetClipboardData(CF_UNICODE_TEXT, handle) orelse return error.SetClipboardData;
}

pub fn getOrEnableAnsiEscapeSupport(fileType: platform.FileType) bool {
    const ENABLE_VIRTUAL_TERMINAL_PROCESSING = 0x0004;
    const STD_INPUT_HANDLE: windows.DWORD = @bitCast(@as(i32, -10));
    const STD_OUTPUT_HANDLE: windows.DWORD = @bitCast(@as(i32, -11));
    const STD_ERROR_HANDLE: windows.DWORD = @bitCast(@as(i32, -12));

    const handleType = switch (fileType) {
        .stdin => STD_INPUT_HANDLE,
        .stdout => STD_OUTPUT_HANDLE,
        .stderr => STD_ERROR_HANDLE,
    };

    if (GetStdHandle(handleType)) |handle| {
        var mode: windows.DWORD = 0;

        if (GetConsoleMode(handle, &mode) == .FALSE) {
            return false; // not a console (maybe redirected)
        }

        const new_mode = mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING;

        if (SetConsoleMode(handle, new_mode) == .FALSE) {
            return false;
        }

        return true;
    }

    return false;
}
