@echo off
pushd "%~dp0"
zig build --prefix build/zig --cache-dir build/.zig-cache %*
popd
exit /b %errorlevel%
