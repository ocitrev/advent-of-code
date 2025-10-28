@if not exist "%~dp0build\cpp" call "%~dp0configure-cpp.cmd"
@set "NINJA_STATUS=[%%s/%%t %%p :: %%e] "
cmake --build "%~dp0build\cpp" %*
@exit /b %errorlevel%
