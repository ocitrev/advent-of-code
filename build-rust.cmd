@echo off
pushd "%~dp0"
cargo build %*
popd
exit /b %errorlevel%
