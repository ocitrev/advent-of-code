:: make sure cmake is installed
@where.exe cmake 2>&1 >NUL
@if errorlevel 1 (
	echo cmake not found! >&2
	exit /b 1
)

:: make sure ninja is installed
@where.exe ninja 2>&1 >NUL
@if errorlevel 1 (
	echo ninja not found! >&2
	exit /b 1
)

:: make sure VCPKG is setup
@if "%VCPKG_ROOT%" == "" (
	echo VCPKG_ROOT is not defined >&2
	exit /b 1
)

set CMAKE_FLAGS=-DCMAKE_C_COMPILER="zig;cc" -DCMAKE_CXX_COMPILER="zig;c++"
cmake -G "Ninja Multi-Config" -S "%~dp0." -B "%~dp0build\cpp" %CMAKE_FLAGS% %*
@exit /b %errorlevel%
