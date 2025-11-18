@echo off

where.exe zig 2>&1 >NUL
if errorlevel 1 (
    echo Zig not installed! >&2
    exit /b 1
)

zig build --prefix "%~dp0build/zig" --cache-dir "%~dp0build/.zig-cache" %*
exit /b %errorlevel%
