@if not exist "%~dp0build\cpp" call "%~dp0configure-cpp.cmd"
cmake --build "%~dp0build\cpp" %*
@exit /b %errorlevel%
